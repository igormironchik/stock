
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "tcp_socket.hpp"
#include "buffer.hpp"
#include "constants.hpp"

// Qt include.
#include <QDataStream>
#include <QTextStream>

// cfgfile include.
#include <cfgfile/all.hpp>

// OpenSSL include.
#include <openssl/evp.h>
#include <openssl/aes.h>


namespace Stock {

//
// MsgType
//

//! Type of the message.
enum class MsgType {
	//! Unknown.
	Unknown = 0,
	//! Add product.
	AddProduct = 1,
	//! Give list of products.
	GiveListOfProducts = 2,
	//! List of products.
	ListOfProducts = 3,
	//! Error.
	Error = 4,
	//! Hello.
	Hello = 5,
	//! Ok.
	Ok = 6
}; // enum class MsgType


template< typename T >
quint16 msgType();

template<>
quint16 msgType< Stock::Messages::AddProduct > ()
{
	return static_cast< quint16 > ( MsgType::AddProduct );
}

template<>
quint16 msgType< Stock::Messages::GiveListOfProducts > ()
{
	return static_cast< quint16 > ( MsgType::GiveListOfProducts );
}

template<>
quint16 msgType< Stock::Messages::ListOfProducts > ()
{
	return static_cast< quint16 > ( MsgType::ListOfProducts );
}

template<>
quint16 msgType< Stock::Messages::Error > ()
{
	return static_cast< quint16 > ( MsgType::Error );
}

template<>
quint16 msgType< Stock::Messages::Hello > ()
{
	return static_cast< quint16 > ( MsgType::Hello );
}

template<>
quint16 msgType< Stock::Messages::Ok > ()
{
	return static_cast< quint16 > ( MsgType::Ok );
}

//! Timeout in seconds for messages with result.
static const int c_timeout = 15;


//
// TcpSocketPrivate
//

class TcpSocketPrivate {
public:
	TcpSocketPrivate( const QString & pwd, TcpSocket * parent )
		:	m_pwd( pwd )
		,	q( parent )
	{
		if( !m_pwd.isEmpty() )
			aesInit( m_pwd );
	}
	
	~TcpSocketPrivate()
	{
		aesDeinit();
	}

	//! Parse messages. \return false on error.
	bool parse();
	
	//! Deinit AES if it's initialized.
	void aesDeinit()
	{
		if( m_eCtx )
		{
			EVP_CIPHER_CTX_free( m_eCtx );
			m_eCtx = nullptr;
		}
		
		if( m_dCtx )
		{
			EVP_CIPHER_CTX_free(  m_dCtx );
			m_dCtx = nullptr;
		}
	}
	
	/*!
		Create a 256 bit key and IV using the supplied key_data. salt can be added for taste.
		Fills in the encryption and decryption ctx objects and returns 0 on success
	*/
	int aesInit( const QString & pwd )
	{
		aesDeinit();
		
		const auto pd = pwd.toUtf8();
		
		const auto keyData = reinterpret_cast< const unsigned char * > ( pd.data() );
		int keyDataLen = pd.size();
		
		int i, nrounds = 5;
		unsigned char key[ 32 ], iv[ 32 ];
		
		/*
			Gen key & IV for AES 256 CBC mode. A SHA1 digest is used to hash the supplied key material.
			nrounds is the number of times the we hash the material. More rounds are more secure but
			slower.
		*/
		i = EVP_BytesToKey( EVP_aes_256_cbc(), EVP_sha1(), m_salt, keyData, keyDataLen,
			nrounds, key, iv );
		
		if( i != 32 )
			return -1;
		
		m_eCtx = EVP_CIPHER_CTX_new();
		EVP_EncryptInit_ex( m_eCtx, EVP_aes_256_cbc(), NULL, key, iv );
		m_dCtx = EVP_CIPHER_CTX_new();
		EVP_DecryptInit_ex( m_dCtx, EVP_aes_256_cbc(), NULL, key, iv );
		
		return 0;
	}
	
	QByteArray aesEncrypt( const QByteArray & data )
	{	
		const auto plainText = reinterpret_cast< const unsigned char * > ( data.data() );
		int len = data.size();
		
		/* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
		int cLen = len + AES_BLOCK_SIZE;
		int fLen = 0;
		std::string cipherText( cLen, 0x00 );
		
		/* allows reusing of 'e' for multiple encryption cycles */
		EVP_EncryptInit_ex( m_eCtx, NULL, NULL, NULL, NULL );
		
		/* update ciphertext, c_len is filled with the length of ciphertext generated,
			len is the size of plaintext in bytes */
		EVP_EncryptUpdate( m_eCtx, reinterpret_cast< unsigned char * > ( cipherText.data() ), &cLen,
			plainText, len );
		
		/* update ciphertext with the final remaining bytes */
		EVP_EncryptFinal_ex( m_eCtx, reinterpret_cast< unsigned char * > ( cipherText.data() ) + cLen,
			&fLen );
		
		return QByteArray( cipherText.c_str(), cLen + fLen );
	}
	
	QByteArray aesDecrypt( const QByteArray & data )
	{
		const auto cipherText = reinterpret_cast< const unsigned char * > ( data.data() );
		int len = data.size();
		
		/* plaintext will always be equal to or lesser than length of ciphertext*/
		int pLen = data.size();
		int fLen = 0;
		std::string plainText( pLen, 0x00 );
		
		EVP_DecryptInit_ex( m_dCtx, NULL, NULL, NULL, NULL );
		EVP_DecryptUpdate( m_dCtx, reinterpret_cast< unsigned char * > ( plainText.data() ), &pLen,
			cipherText, len );
		EVP_DecryptFinal_ex( m_dCtx, reinterpret_cast< unsigned char * > ( plainText.data() ) + pLen,
			&fLen );
		
		return QByteArray( plainText.c_str(), pLen + fLen );
	}

	//! Buffer.
	Buffer m_buf;
	//! Password.
	QString m_pwd;
	//! Parent.
	TcpSocket * q;
	//! Encoding context.
	EVP_CIPHER_CTX * m_eCtx = nullptr;
	//! Decoding context.
	EVP_CIPHER_CTX * m_dCtx = nullptr;
	//! Salt.
	static const unsigned char m_salt[ 8 ];
}; // class TcpSocketPrivate

const unsigned char TcpSocketPrivate::m_salt[ 8 ] = { 0x01, 0x06, 0x03, 0x0A, 0xFF, 0xAE, 0x01, 0x08 };

bool
TcpSocketPrivate::parse()
{
	if( m_dCtx )
	{
		while( !m_buf.isEmpty() )
		{
			QDataStream s( m_buf.data() );
			s.setVersion( QDataStream::Qt_5_9 );
	
			quint64 magic = 0;
			quint16 type = 0;
			qint32 length = 0;
	
			s >> magic >> type >> length;
	
			if( s.status() != QDataStream::Ok )
				return true;
	
			if( magic != c_magic )
				return false;
	
			QByteArray msgData( length, 0 );
	
			if( s.readRawData( msgData.data(), length ) != length )
				return true;
	
			msgData = aesDecrypt( QByteArray::fromBase64( msgData ) );
	
			QTextStream msgStream( msgData );
	
			switch( static_cast< MsgType > ( type ) )
			{
				case MsgType::AddProduct :
				{
					try {
						Messages::tag_AddProduct< cfgfile::qstring_trait_t > tag;
	
						cfgfile::read_cfgfile( tag, msgStream, QLatin1String( "Network Data" ) );
	
						emit q->addProduct( tag.get_cfg() );
					}
					catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
					{
						return false;
					}
				}
					break;
	
				case MsgType::GiveListOfProducts :
				{
					try {
						Messages::tag_GiveListOfProducts< cfgfile::qstring_trait_t > tag;
	
						cfgfile::read_cfgfile( tag, msgStream, QLatin1String( "Network Data" ) );
	
						emit q->giveListOfProducts( tag.get_cfg() );
					}
					catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
					{
						return false;
					}
				}
					break;
	
				case MsgType::ListOfProducts :
				{
					try {
						Messages::tag_ListOfProducts< cfgfile::qstring_trait_t > tag;
	
						cfgfile::read_cfgfile( tag, msgStream, QLatin1String( "Network Data" ) );
	
						emit q->listOfProducts( tag.get_cfg() );
					}
					catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
					{
						return false;
					}
				}
					break;
	
				case MsgType::Error :
				{
					try {
						Messages::tag_Error< cfgfile::qstring_trait_t > tag;
	
						cfgfile::read_cfgfile( tag, msgStream, QLatin1String( "Network Data" ) );
	
						emit q->error( tag.get_cfg() );
					}
					catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
					{
						return false;
					}
				}
					break;
	
				case MsgType::Hello :
				{
					try {
						Messages::tag_Hello< cfgfile::qstring_trait_t > tag;
	
						cfgfile::read_cfgfile( tag, msgStream, QLatin1String( "Network Data" ) );
	
						emit q->hello( tag.get_cfg() );
					}
					catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
					{
						return false;
					}
				}
					break;
	
				case MsgType::Ok :
				{
					try {
						Messages::tag_Ok< cfgfile::qstring_trait_t > tag;
	
						cfgfile::read_cfgfile( tag, msgStream, QLatin1String( "Network Data" ) );
	
						emit q->ok( tag.get_cfg() );
					}
					catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
					{
						return false;
					}
				}
					break;
	
				default :
					return false;
			}
	
			m_buf.remove( s.device()->pos() );
		}
		
		return true;
	}
	
	return false;
}


//
// TcpSocket
//

TcpSocket::TcpSocket( const QString & pwd, QObject * parent )
	:	QTcpSocket( parent )
	,	d( new TcpSocketPrivate( pwd, this ) )
{
	connect( this, &QTcpSocket::readyRead,
		this, &TcpSocket::dataReceived );
}

TcpSocket::~TcpSocket()
{
}

void
TcpSocket::sendHello( const Stock::Messages::Hello & msg )
{
	sendMsg< Stock::Messages::Hello,
		Stock::Messages::tag_Hello< cfgfile::qstring_trait_t > > ( msg );
}

void
TcpSocket::sendError( const Stock::Messages::Error & msg )
{
	sendMsg< Stock::Messages::Error,
		Stock::Messages::tag_Error< cfgfile::qstring_trait_t > > ( msg );
}

void
TcpSocket::sendOk( const Stock::Messages::Ok & msg )
{
	sendMsg< Stock::Messages::Ok,
		Stock::Messages::tag_Ok< cfgfile::qstring_trait_t > > ( msg );
}

void
TcpSocket::sendListOfProducts( Stock::Messages::ListOfProducts & msg )
{
	sendMsg< Stock::Messages::ListOfProducts,
		Stock::Messages::tag_ListOfProducts< cfgfile::qstring_trait_t > > ( msg );
}

void
TcpSocket::sendGiveListOfProducts( Stock::Messages::GiveListOfProducts & msg )
{
	sendMsg< Stock::Messages::GiveListOfProducts,
		Stock::Messages::tag_GiveListOfProducts< cfgfile::qstring_trait_t > > ( msg );
}

void
TcpSocket::sendAddProduct( Stock::Messages::AddProduct & msg )
{
	sendMsg< Stock::Messages::AddProduct,
		Stock::Messages::tag_AddProduct< cfgfile::qstring_trait_t > > ( msg );
}

template< typename MSG, typename TAG >
void
TcpSocket::sendMsg( const MSG & msg )
{
	if( d->m_eCtx )
	{
		try {
			QByteArray data;
			QTextStream stream( &data, QIODevice::WriteOnly );
	
			TAG tag( msg );
	
			cfgfile::write_cfgfile( tag, stream );
			stream.flush();
	
			data = d->aesEncrypt( data ).toBase64();
	
			QByteArray msgData;
			QDataStream s( &msgData, QIODevice::WriteOnly );
			s.setVersion( QDataStream::Qt_5_9 );
	
			s << c_magic << msgType< MSG > ()
				<< static_cast< qint32 > ( data.size() );
			s.writeRawData( data.constData(), data.size() );
	
			writeData( msgData.constData(), msgData.size() );
	
			flush();
		}
		catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
		{
			d->m_buf.clear();
	
			disconnectFromHost();
		}
	}
	else
	{
		d->m_buf.clear();
		
		disconnectFromHost();
	}
}

void
TcpSocket::dataReceived()
{
	d->m_buf.write( readAll() );

	if( !d->parse() )
	{
		d->m_buf.clear();

		disconnectFromHost();
	}
}

void
TcpSocket::timeout()
{
	emit error( Stock::Messages::Error() );
}

void
TcpSocket::setPwd( const QString & pwd )
{
	d->m_pwd = pwd;

	if( !d->m_pwd.isEmpty() )	
		d->aesInit( d->m_pwd );
}

} /* namespace Stock */
