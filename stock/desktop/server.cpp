
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
#include "server.hpp"
#include "shared/tcp_socket.hpp"
#include "db.hpp"
#include "db_signals.hpp"
#include "by_product_model.hpp"
#include "by_place_model.hpp"

// Qt include.
#include <QVector>


namespace Stock {

//
// ServerPrivate
//

class ServerPrivate {
public:
	ServerPrivate( Server * parent )
		:	m_db( Q_NULLPTR )
		,	m_sigs( Q_NULLPTR )
		,	m_codeModel( Q_NULLPTR )
		,	m_placeModel( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Clients.
	QVector< TcpSocket* > m_clients;
	//! Secret code.
	QString m_secret;
	//! DB.
	Db * m_db;
	//! DB signals.
	DbSignals * m_sigs;
	//! By product model.
	ByProductModel * m_codeModel;
	//! By place mode.
	ByPlaceModel * m_placeModel;
	//! Parent.
	Server * q;
}; // class ServerPrivate

void
ServerPrivate::init()
{

}


//
// Server
//

Server::Server( QObject * parent )
	:	QTcpServer( parent )
	,	d( new ServerPrivate( this ) )
{
	d->init();
}

Server::~Server()
{
}

void
Server::setDbAndModels( Db * db, DbSignals * sigs,
	ByProductModel * codeModel, ByPlaceModel * placeModel )
{
	d->m_db = db;
	d->m_sigs = sigs;
	d->m_codeModel = codeModel;
	d->m_placeModel = placeModel;
}

const QString &
Server::secret() const
{
	return d->m_secret;
}

void
Server::setSecret( const QString & secret )
{
	d->m_secret = secret;
}

template< typename T >
std::vector< T > toStdVector( const QList< T > & list )
{
	return std::vector< T > ( list.constBegin(), list.constEnd() );
}

void
Server::incomingConnection( qintptr socketDescriptor )
{
	TcpSocket * socket = new TcpSocket( this );

	if( socket->setSocketDescriptor( socketDescriptor ) )
	{
		connect( socket, &TcpSocket::disconnected,
			this, &Server::clientDisconnected,
			Qt::QueuedConnection );
		connect( socket, &TcpSocket::addProduct,
			this, &Server::addProduct,
			Qt::QueuedConnection );
		connect( socket, &TcpSocket::giveListOfProducts,
			this, &Server::giveListOfProducts,
			Qt::QueuedConnection );
		connect( socket, &TcpSocket::errorOccurred,
			socket, &TcpSocket::disconnectFromHost );

		d->m_clients.push_back( socket );

		Stock::Messages::Hello msg;
		msg.set_places( toStdVector( d->m_placeModel->places() ) );
		msg.set_products( toStdVector( d->m_codeModel->codes() ) );

		socket->sendHello( msg );
	}
	else
		delete socket;
}

void
Server::addProduct( const Stock::Messages::AddProduct & msg )
{
	if( msg.secret() == d->m_secret && !msg.code().isEmpty() &&
		!msg.place().isEmpty() && msg.count() != 0 )
	{
		const quint64 inDb = d->m_codeModel->count( msg.code(), msg.place() );

		quint64 count = 0;

		if( msg.count() > 0 || ( msg.count() < 0 &&
			static_cast< quint64 > ( qAbs( msg.count() ) ) <= inDb ) )
				count = msg.count() + inDb;

		const auto desc = d->m_codeModel->desc( msg.code() );

		DbResult res = d->m_db->changeProduct( { msg.code(), msg.place(),
			count, desc } );

		if( res.m_ok )
		{
			static_cast< TcpSocket* > ( sender() )->sendOk( Stock::Messages::Ok() );

			d->m_sigs->emitProductChanged( msg.code(), msg.place(), count, desc );
		}
		else
			static_cast< TcpSocket* > ( sender() )->sendError( Stock::Messages::Error() );
	}
	else
		static_cast< TcpSocket* > ( sender() )->sendError( Stock::Messages::Error() );
}

void
Server::giveListOfProducts( const Stock::Messages::GiveListOfProducts & msg )
{
	if( msg.secret() == d->m_secret && ( !msg.code().isEmpty() || !msg.place().isEmpty() ) )
	{
		QVector< DbRecord > records;

		if( !msg.code().isEmpty() )
			records = d->m_codeModel->records( msg.code() );
		else if( !msg.place().isEmpty() )
			records = d->m_placeModel->records( msg.place() );

		Stock::Messages::ListOfProducts list;
		std::vector< Stock::Messages::Product > prods;

		for( const auto & r : qAsConst( records ) )
		{
			Stock::Messages::Product p;
			p.set_code( r.m_code );
			p.set_count( r.m_count );
			p.set_desc( r.m_desc );
			p.set_place( r.m_place );

			prods.push_back( p );
		}

		list.set_product( prods );

		static_cast< TcpSocket* > ( sender() )->sendListOfProducts( list );
	}
	else
		static_cast< TcpSocket* > ( sender() )->sendError( Stock::Messages::Error() );
}

void
Server::clientDisconnected()
{
	disconnect( sender(), 0, this, 0 );

	d->m_clients.removeOne( static_cast< TcpSocket* > ( sender() ) );

	delete sender();
}

} /* namespace Stock */
