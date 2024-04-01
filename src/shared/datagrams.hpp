
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_DATAGRAMS_HPP_INCLUDED
#define STOCK_DATAGRAMS_HPP_INCLUDED

// Qt include.
#include <QNetworkDatagram>
#include <QDataStream>
#include <QTextStream>
#include <QByteArray>

// cfgfile include.
#include <cfgfile/all.hpp>

// Stock include.
#include "messages.hpp"
#include "constants.hpp"
#include "exceptions.hpp"

QT_BEGIN_NAMESPACE
class QUdpSocket;
QT_END_NAMESPACE


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
inline
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
inline
DatagramType datagramType( const Messages::MyIP & )
{
	return DatagramType::MyIP;
}

template<>
inline
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
writeTellIpDatargam( QUdpSocket * s, quint16 port = c_udpPort );


//
// writeMyIpDatargam
//

//! Write MyIP datagram.
void
writeMyIpDatargam( QUdpSocket * s, const QString & host, quint16 port,
	const QHostAddress & receiverHost, quint16 receiverPort );

} /* namespace Stock */

#endif // STOCK_DATAGRAMS_HPP_INCLUDED
