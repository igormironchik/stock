
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "network.hpp"
#include "qml_cpp_bridge.hpp"
#include "shared/tcp_socket.hpp"
#include "shared/datagrams.hpp"

// Qt include.
#include <QUdpSocket>
#include <QTimer>

// C++ include.
#include <algorithm>


namespace Stock {

//
// NetworkPrivate
//

class NetworkPrivate {
public:
	NetworkPrivate( QmlCppBridge * sigs, Network * parent )
		:	m_connected( false )
		,	m_disconnectRequestedByUser( false )
		,	m_port( 0 )
		,	m_udpPort( c_udpPort )
		,	m_sigs( sigs )
		,	m_sock( Q_NULLPTR )
		,	m_udp( Q_NULLPTR )
		,	m_timer( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Connected?
	bool m_connected;
	//! Disconnected reuquested by user.
	bool m_disconnectRequestedByUser;
	//! IP.
	QString m_ip;
	//! Password.
	QString m_pwd;
	//! Port.
	quint16 m_port;
	//! UDP port.
	quint16 m_udpPort;
	//! Qml to C++ bridge.
	QmlCppBridge * m_sigs;
	//! TCP socket.
	TcpSocket * m_sock;
	//! UDP socket.
	QUdpSocket * m_udp;
	//! Timer.
	QTimer * m_timer;
	//! Parent.
	Network * q;
}; // class NetworkPrivate

//! Max network timeout.
static const int c_timeout = 15;

void
NetworkPrivate::init()
{
	m_sock = new TcpSocket( m_pwd, q );
	m_udp = new QUdpSocket(	q );
	m_timer = new QTimer( q );
	m_timer->setSingleShot( true );
	m_timer->setInterval( c_timeout * 1000 );

	QObject::connect( m_udp, &QUdpSocket::readyRead,
		q, &Network::readPendingDatagrams );
	QObject::connect( m_sock, &TcpSocket::disconnected,
		q, &Network::networkDisconnected );
	QObject::connect( m_timer, &QTimer::timeout,
		q, &Network::timeout );
	QObject::connect( m_sock, &TcpSocket::hello,
		q, &Network::hello );
	QObject::connect( m_sock, &TcpSocket::ok,
		q, &Network::operationSuccessful );
	QObject::connect( m_sock, &TcpSocket::error,
		q, &Network::serverError );
}


//
// Network
//

Network::Network( QmlCppBridge * sigs )
	:	QObject( sigs )
	,	d( new NetworkPrivate( sigs, this ) )
{
	d->init();
}

Network::~Network()
{
}

const QString &
Network::password() const
{
	return d->m_pwd;
}

void
Network::setPassword( const QString & pwd )
{
	d->m_pwd = pwd;
	d->m_sock->setPwd( pwd );
}

quint16
Network::udpPort() const
{
	return d->m_udpPort;
}

void
Network::setUdpPort( quint16 port )
{
	d->m_udpPort = port;
}

TcpSocket *
Network::socket() const
{
	return d->m_sock;
}

void
Network::establishConnection()
{
	d->m_timer->start();

	d->m_connected = false;
	d->m_ip.clear();
	d->m_port = 0;

	d->m_sock->disconnectFromHost();

	d->m_disconnectRequestedByUser = false;
	emit disconnected( false );

	d->m_udp->bind();

	writeTellIpDatargam( d->m_udp, d->m_udpPort );
}

void
Network::disconnectNetwork( bool requestedByUser )
{
	d->m_disconnectRequestedByUser = requestedByUser;
	d->m_timer->stop();
	d->m_sock->disconnectFromHost();
	d->m_udp->close();
	d->m_connected = false;
}

void
Network::readPendingDatagrams()
{
	while( d->m_udp->hasPendingDatagrams() )
	{
		QNetworkDatagram datagram = d->m_udp->receiveDatagram();

		if( datagramType( datagram ) == DatagramType::MyIP && !d->m_connected )
		{
			Messages::MyIP msg;

			try {
				readDatagram< Messages::MyIP,
					Messages::tag_MyIP< cfgfile::qstring_trait_t > > (
						datagram, msg );

				d->m_ip = msg.ip();
				d->m_port = msg.port();

				d->m_udp->close();

				d->m_sock->connectToHost( QHostAddress( d->m_ip ), d->m_port );

				d->m_timer->start();

				break;
			}
			catch( const Exception & )
			{
				disconnectNetwork();

				emit error();
			}
		}
	}
}

void
Network::networkDisconnected()
{
	d->m_connected = false;

	emit disconnected( d->m_disconnectRequestedByUser );
}

void
Network::timeout()
{
	disconnectNetwork();

	emit error();
}

void
Network::serverError( const Stock::Messages::Error & )
{
	emit opFailed();
}

void
Network::operationSuccessful( const Stock::Messages::Ok & )
{
	emit opOk();
}

void
Network::hello( const Stock::Messages::Hello & msg )
{
	d->m_timer->stop();

	QStringList codes;
	QStringList places;

	std::for_each( msg.products().cbegin(), msg.products().cend(),
		[&] ( const auto & str ) { codes.push_back( str ); } );

	std::for_each( msg.places().cbegin(), msg.places().cend(),
		[&] ( const auto & str ) { places.push_back( str ); } );

	d->m_connected = true;

	emit connected( codes, places );
}

} /* namespace Stock */
