
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
