
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_NETWORK_HPP_INCLUDED
#define STOCK_NETWORK_HPP_INCLUDED

// Qt include.
#include <QObject>

// C++ include.
#include <memory>

// Stock include.
#include "messages.hpp"


namespace Stock {

class QmlCppBridge;
class TcpSocket;

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
    void disconnected( bool requestedByUser = false );
	//! Connected.
	void connected( const QStringList & codes, const QStringList & places );
	//! Error.
	void error();
	//! Operation ok.
	void opOk();
	//! Operation failed.
	void opFailed();

public:
	explicit Network( QmlCppBridge * sigs );
	virtual ~Network();

	//! \return Current password.
	const QString & password() const;
	//! Set pasword.
	void setPassword( const QString & pwd );

	//! \return UDP port.
	quint16 udpPort() const;
	//! Set UDP port.
	void setUdpPort( quint16 port );

	//! \return TCP socket.
	TcpSocket * socket() const;

public slots:
	//! Establish connection.
	void establishConnection();
	//! Disconnect network.
    void disconnectNetwork( bool requestedByUser = false );

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

	std::unique_ptr< NetworkPrivate > d;
}; // class Network

} /* namespace Stock */

#endif // STOCK_NETWORK_HPP_INCLUDED
