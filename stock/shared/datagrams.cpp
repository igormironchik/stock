
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
#include "datagrams.hpp"
#include "constants.hpp"
#include "exceptions.hpp"

// Qt include.
#include <QByteArray>
#include <QDataStream>
#include <QTextStream>
#include <QTextCodec>

// cfgfile include.
#include <cfgfile/all.hpp>


namespace Stock {

//
// datagramType
//

DatagramType datagramType( const QNetworkDatagram & d )
{
	const QByteArray data = QByteArray::fromBase64( d.data() );
	QDataStream s( data );
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


//
// readDatagram
//

void readDatagram( const QNetworkDatagram & d, Messages::MyIP & msg )
{
	const QByteArray data = QByteArray::fromBase64( d.data() );
	QDataStream s( data );
	s.setVersion( QDataStream::Qt_5_9 );

	quint64 magic = 0;
	s >> magic;

	if( s.status() != QDataStream::Ok || magic != c_magic )
		throw Exception( QObject::tr( "Wrong protocol" ) );

	quint32 type = 0;
	s >> type;

	if( s.status() != QDataStream::Ok &&
		static_cast< DatagramType > ( type ) != DatagramType::MyIP )
			throw Exception( QObject::tr(
				"readDatagram(): not Messages::MyIP message" ) );

	QString txt;
	s >> txt;

	QTextStream stream( &txt, QIODevice::ReadOnly );
	stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

	try {
		Messages::tag_MyIP< cfgfile::qstring_trait_t > tag;

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

void writeTellIpDatargam( QUdpSocket * s )
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
		Messages::tag_TellMeYourIP< cfgfile::qstring_trait_t > tag( msg );

		cfgfile::write_cfgfile( tag, text );

		stream << str;

		s->writeDatagram( QNetworkDatagram( array ) );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		throw Exception( x.desc() );
	}
}


//
// writeMyIpDatargam
//

void writeMyIpDatargam( QUdpSocket * s, const QString & host, quint16 port )
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

		s->writeDatagram( QNetworkDatagram( array ) );
	}
	catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
	{
		throw Exception( x.desc() );
	}
}

} /* namespace Stock */
