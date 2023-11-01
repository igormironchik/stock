
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
