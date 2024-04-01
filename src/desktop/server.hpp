
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_SERVER_HPP_INCLUDED
#define STOCK_SERVER_HPP_INCLUDED

// Qt include.
#include <QTcpServer>

// C++ include.
#include <memory>


namespace Stock {

namespace Messages {

class AddProduct;
class GiveListOfProducts;
class ListOfProducts;

} /* namespace Messages */


class Db;
class DbSignals;
class ByProductModel;
class ByPlaceModel;


//
// Server
//

class ServerPrivate;

//! TCP server.
class Server Q_DECL_FINAL
	:	public QTcpServer
{
	Q_OBJECT

public:
	explicit Server( QObject * parent );
	virtual ~Server();

	//! Set DB and models.
	void setDbAndModels( Db * db, DbSignals * sigs,
		ByProductModel * codeModel, ByPlaceModel * placeModel );

	//! \return Secret.
	const QString & secret() const;
	//! Set secret.
	void setSecret( const QString & secret );

protected:
	//!	Process new incoming connection.
	void incomingConnection( qintptr socketDescriptor ) Q_DECL_OVERRIDE;

private slots:
	//! Add/edit product.
	void addProduct( const Stock::Messages::AddProduct & msg );
	//! Give list of products on place.
	void giveListOfProducts( const Stock::Messages::GiveListOfProducts & msg );
	//! Client disconnected.
	void clientDisconnected();

private:
	Q_DISABLE_COPY( Server )

	std::unique_ptr< ServerPrivate > d;
}; // class Server

} /* namespace Stock */

#endif // STOCK_SERVER_HPP_INCLUDED
