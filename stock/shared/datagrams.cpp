
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

// Qt include.
#include <QUdpSocket>

// Stock include.
#include "datagrams.hpp"


namespace Stock {

//
// writeTellIpDatargam
//

//! Write TellIP datagram.
void
writeTellIpDatargam( QUdpSocket * s, quint16 port )
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
