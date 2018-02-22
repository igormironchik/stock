
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

#ifndef STOCK_TCP_COCKET_HPP_INCLUDED
#define STOCK_TCP_COCKET_HPP_INCLUDED

// Qt include.
#include <QTcpSocket>
#include <QScopedPointer>


namespace Stock {

//
// TcpSocket
//

class TcpSocketPrivate;

//! TCP socket.
class TcpSocket Q_DECL_FINAL
	:	public QTcpSocket
{
	Q_OBJECT

public:
	explicit TcpSocket( QObject * parent );
	virtual ~TcpSocket();

private slots:
	//! Data received.
	void dataReceived();

private:
	Q_DISABLE_COPY( TcpSocket )

	QScopedPointer< TcpSocketPrivate > d;
}; // class TcpSocket

} /* namespace Stock */

#endif // STOCK_TCP_COCKET_HPP_INCLUDED
