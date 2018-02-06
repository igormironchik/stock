
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

#ifndef STOCK_DATAGRAMS_HPP_INCLUDED
#define STOCK_DATAGRAMS_HPP_INCLUDED

// Qt include.
#include <QNetworkDatagram>
#include <QUdpSocket>

// Stock include.
#include "messages.hpp"


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
DatagramType datagramType( const QNetworkDatagram & d );


//
// readDatagram
//

//! Read MyIP datagram.
void readDatagram( const QNetworkDatagram & d, Messages::MyIP & msg );


//
// writeTellIpDatargam
//

//! Write TellIP datagram.
void writeTellIpDatargam( QUdpSocket * s );


//
// writeMyIpDatargam
//

//! Write MyIP datagram.
void writeMyIpDatargam( QUdpSocket * s, const QString & host, quint16 port );

} /* namespace Stock */

#endif // STOCK_DATAGRAMS_HPP_INCLUDED
