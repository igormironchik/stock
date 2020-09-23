
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

#ifndef STOCK_DATAGRAMS_HPP_INCLUDED
#define STOCK_DATAGRAMS_HPP_INCLUDED

// Qt include.
#include <QNetworkDatagram>
#include <QUdpSocket>
#include <QDataStream>
#include <QTextStream>
#include <QTextCodec>
#include <QByteArray>

// cfgfile include.
#include <cfgfile/all.hpp>

// Stock include.
#include "messages.hpp"
#include "constants.hpp"
#include "exceptions.hpp"


namespace Stock {

//
// DatagramType
//

//! Datagram type.
enum class DatagramType {
	Unknown = 0,
	TellIP = 1,
	MyIP = 2
}; // enum class DatagramType


//
// datagramType
//

//! \return Type of the datagram.
template< typename MSG >
DatagramType datagramType( const MSG & );

template<>
DatagramType datagramType( const QNetworkDatagram & d )
{
	QDataStream s( d.data() );
	s.setVersion( QDataStream::Qt_5_9 );

	quint64 magic = 0;
	s >> magic;

	if( s.status() != QDataStream::Ok || magic != c_magic )
		return DatagramType::Unknown;

	quint32 type = 0;
	s >> type;

	if( s.status() != QDataStream::Ok )
		return DatagramType::Unknown;

	switch( static_cast< DatagramType > ( type ) )
	{
		case DatagramType::MyIP :
			return DatagramType::MyIP;

		case DatagramType::TellIP :
			return DatagramType::TellIP;

		default :
			return DatagramType::Unknown;
	}
}

template<>
DatagramType datagramType( const Messages::MyIP & )
{
	return DatagramType::MyIP;
}

template<>
DatagramType datagramType( const Messages::TellMeYourIP & )
{
	return DatagramType::TellIP;
}


//
// readDatagram
//

template< typename MSG, typename TAG >
void readDatagram( const QNetworkDatagram & d, MSG & msg )
{
	QDataStream s( d.data() );
	s.setVersion( QDataStream::Qt_5_9 );

	quint64 magic = 0;
	s >> magic;

	if( s.status() != QDataStream::Ok || magic != c_magic )
		throw Exception( QObject::tr( "Wrong protocol" ) );

	quint32 type = 0;
	s >> type;

	if( s.status() != QDataStream::Ok ||
		static_cast< DatagramType > ( type ) != datagramType( msg ) )
			throw Exception( QObject::tr(
				"readDatagram(): wrong datagram" ) );

	QString txt;
	s >> txt;

	QTextStream stream( &txt, QIODevice::ReadOnly );
	stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

	try {
		TAG tag;

		cfgfile::read_cfgfile( tag, stream, QLatin1String( "udp" ) );

		msg = tag.get_cfg();
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		throw Exception( x.desc() );
	}
}


//
// writeTellIpDatargam
//

//! Write TellIP datagram.
void
writeTellIpDatargam( QUdpSocket * s, const QString & password, quint16 port = c_udpPort )
{
	QByteArray array;
	QDataStream stream( &array, QIODevice::WriteOnly );
	stream.setVersion( QDataStream::Qt_5_9 );

	stream << c_magic;
	quint32 type = static_cast< quint32 > ( DatagramType::TellIP );
	stream << type;

	try {
		QString str;
		QTextStream text( &str, QIODevice::WriteOnly );
		text.setCodec( QTextCodec::codecForName( "UTF-8" ) );

		Messages::TellMeYourIP msg;
		msg.set_secret( password );
		Messages::tag_TellMeYourIP< cfgfile::qstring_trait_t > tag( msg );

		cfgfile::write_cfgfile( tag, text );

		stream << str;

		s->writeDatagram( array, QHostAddress::Broadcast, port );

		s->flush();
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		throw Exception( x.desc() );
	}
}



//
// writeMyIpDatargam
//

//! Write MyIP datagram.
void
writeMyIpDatargam( QUdpSocket * s, const QString & host, quint16 port,
	const QHostAddress & receiverHost, quint16 receiverPort )
{
	QByteArray array;
	QDataStream stream( &array, QIODevice::WriteOnly );
	stream.setVersion( QDataStream::Qt_5_9 );

	stream << c_magic;
	quint32 type = static_cast< quint32 > ( DatagramType::MyIP );
	stream << type;

	try {
		QString str;
		QTextStream text( &str, QIODevice::WriteOnly );
		text.setCodec( QTextCodec::codecForName( "UTF-8" ) );

		Messages::MyIP msg;
		msg.set_ip( host );
		msg.set_port( port );
		Messages::tag_MyIP< cfgfile::qstring_trait_t > tag( msg );

		cfgfile::write_cfgfile( tag, text );

		stream << str;

		s->writeDatagram( QNetworkDatagram( array, receiverHost,
			receiverPort ) );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		throw Exception( x.desc() );
	}
}

} /* namespace Stock */

#endif // STOCK_DATAGRAMS_HPP_INCLUDED
