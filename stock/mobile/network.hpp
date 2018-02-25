
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

#ifndef STOCK_NETWORK_HPP_INCLUDED
#define STOCK_NETWORK_HPP_INCLUDED

// Qt include.
#include <QObject>
#include <QScopedPointer>

// Stock include.
#include "messages.hpp"


namespace Stock {

class QmlCppBridge;

//
// Network
//

class NetworkPrivate;

//! Network communications.
class Network Q_DECL_FINAL
	:	public QObject
{
	Q_OBJECT

signals:
	//! Disconnected.
	void disconnected();
	//! Connected.
	void connected( const QStringList & codes, const QStringList & places );
	//! Error.
	void error();
	//! Ok.
	void ok();

public:
	explicit Network( QmlCppBridge * sigs );
	virtual ~Network();

	//! Set pasword.
	void setPassword( const QString & pwd );

public slots:
	//! Establish connection.
	void establishConnection();
	//! Disconnect network.
	void disconnectNetwork();

private slots:
	//! Read UDP datagrams.
	void readPendingDatagrams();
	//! Disconnected.
	void networkDisconnected();
	//! Timeout.
	void timeout();
	//! Error in server.
	void serverError( const Stock::Messages::Error & );
	//! Ok.
	void operationSuccessful( const Stock::Messages::Ok & );
	//! Hello.
	void hello( const Stock::Messages::Hello & msg );

private:
	friend class NetworkPrivate;

	Q_DISABLE_COPY( Network )

	QScopedPointer< NetworkPrivate > d;
}; // class Network

} /* namespace Stock */

#endif // STOCK_NETWORK_HPP_INCLUDED