
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

#ifndef STOCK_TCP_COCKET_HPP_INCLUDED
#define STOCK_TCP_COCKET_HPP_INCLUDED

// Qt include.
#include <QTcpSocket>
#include <QScopedPointer>

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

	QScopedPointer< TcpSocketPrivate > d;
}; // class TcpSocket

} /* namespace Stock */

Q_DECLARE_METATYPE( Stock::Messages::AddProduct )
Q_DECLARE_METATYPE( Stock::Messages::GiveListOfProducts )
Q_DECLARE_METATYPE( Stock::Messages::ListOfProducts )
Q_DECLARE_METATYPE( Stock::Messages::Error )
Q_DECLARE_METATYPE( Stock::Messages::Hello )
Q_DECLARE_METATYPE( Stock::Messages::Ok )

#endif // STOCK_TCP_COCKET_HPP_INCLUDED
