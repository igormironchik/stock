
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_TCP_COCKET_HPP_INCLUDED
#define STOCK_TCP_COCKET_HPP_INCLUDED

// Qt include.
#include <QTcpSocket>

// C++ include.
#include <memory>

// Stock include.
#include "messages.hpp"


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

signals:
	//! Add/edit product.
	void addProduct( const Stock::Messages::AddProduct & );
	//! Give list of products on place.
	void giveListOfProducts( const Stock::Messages::GiveListOfProducts & );
	//! List of products on place.
	void listOfProducts( const Stock::Messages::ListOfProducts & );
	//! Error.
	void error( const Stock::Messages::Error & );
	//! Hello.
	void hello( const Stock::Messages::Hello & );
	//! Ok.
	void ok( const Stock::Messages::Ok & );

public:
	explicit TcpSocket( const std::string & pwd, QObject * parent );
	virtual ~TcpSocket();

	//! Set password.
	void setPwd( const QString & pwd );

public slots:
	//! Send hello.
	void sendHello( const Stock::Messages::Hello & msg );
	//! Send error.
	void sendError( const Stock::Messages::Error & msg );
	//! Send ok.
	void sendOk( const Stock::Messages::Ok & msg );
	//! Send list of products.
	void sendListOfProducts( Stock::Messages::ListOfProducts & msg );
	//! Send "give list of products".
	void sendGiveListOfProducts( Stock::Messages::GiveListOfProducts & msg );
	//! Send "add product".
	void sendAddProduct( Stock::Messages::AddProduct & msg );

private slots:
	//! Data received.
	void dataReceived();
	//! Timeout.
	void timeout();

private:
	//! Send message.
	template< typename MSG, typename TAG >
	void sendMsg( const MSG & msg );

private:
	friend class TcpSocketPrivate;

	Q_DISABLE_COPY( TcpSocket )

	std::unique_ptr< TcpSocketPrivate > d;
}; // class TcpSocket

} /* namespace Stock */

#endif // STOCK_TCP_COCKET_HPP_INCLUDED
