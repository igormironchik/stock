
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018 Igor Mironchik

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
	Hello = 5
}; // enum class MsgType


//
// TcpSocketPrivate
//

class TcpSocketPrivate {
public:
	TcpSocketPrivate( TcpSocket * parent )
		:	q( parent )
	{
	}

	//! Parse messages. \return false on error.
	bool parse();

	//! Buffer.
	Buffer m_buf;
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

TcpSocket::TcpSocket( QObject * parent )
	:	QTcpSocket( parent )
	,	d( new TcpSocketPrivate( this ) )
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
	try {
		QByteArray data;
		QTextStream stream( &data, QIODevice::WriteOnly );
		stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

		Messages::tag_Hello< cfgfile::qstring_trait_t > tag( msg );

		cfgfile::write_cfgfile( tag, stream );

		QByteArray msgData;
		QDataStream s( &msgData, QIODevice::WriteOnly );
		s.setVersion( QDataStream::Qt_5_9 );

		s << c_magic << static_cast< quint16 > ( MsgType::Hello )
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

} /* namespace Stock */
