
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
#include "network.hpp"
#include "qml_cpp_signals.hpp"
#include "shared/tcp_socket.hpp"
#include "shared/datagrams.hpp"

// Qt include.
#include <QUdpSocket>


namespace Stock {

//
// NetworkPrivate
//

class NetworkPrivate {
public:
	NetworkPrivate( QmlCppSignals * sigs, Network * parent )
		:	m_sigs( sigs )
		,	m_sock( Q_NULLPTR )
		,	m_udp( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Qml to C++ bridge.
	QmlCppSignals * m_sigs;
	//! TCP socket.
	TcpSocket * m_sock;
	//! UDP socket.
	QUdpSocket * m_udp;
	//! Parent.
	Network * q;
}; // class NetworkPrivate

void
NetworkPrivate::init()
{

}


//
// Network
//

Network::Network( QmlCppSignals * sigs )
	:	QObject( sigs )
	,	d( new NetworkPrivate( sigs, this ) )
{
	d->init();
}

Network::~Network()
{
}

} /* namespace Stock */
