
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018-2020 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Stock include.
#include "tcp_socket.hpp"
#include "buffer.hpp"
#include "constants.hpp"

// Qt include.
#include <QDataStream>
#include <QTextStream>
#include <QTextCodec>

// cfgfile include.
#include <cfgfile/all.hpp>

// easy-encryption include.
#include <easy-encryption/encrypt.h>


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
	TcpSocketPrivate( const std::string & pwd, TcpSocket * parent )
		:	m_pwd( pwd )
		,	q( parent )
	{
	}

	//! Parse messages. \return false on error.
	bool parse();

	//! Buffer.
	Buffer m_buf;
	//! Password.
	std::string m_pwd;
	//! Parent.
	TcpSocket * q;
}; // class TcpSocketPrivate

bool
TcpSocketPrivate::parse()
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

		auto str = msgData.toStdString();
		const auto data = decrypt( str, m_pwd );

		msgData.clear();
		msgData.append( data.c_str() );

		QTextStream msgStream( msgData );
		msgStream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

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


//
// TcpSocket
//

TcpSocket::TcpSocket( const std::string & pwd, QObject * parent )
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
	try {
		QByteArray data;
		QTextStream stream( &data, QIODevice::WriteOnly );
		stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

		TAG tag( msg );

		cfgfile::write_cfgfile( tag, stream );
		stream.flush();

		auto str = data.toStdString();
		const auto encrypted = encrypt( str, d->m_pwd );
		data.clear();
		data.append( encrypted.c_str() );

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
	d->m_pwd = pwd.toStdString();
}

} /* namespace Stock */
