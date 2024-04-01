
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "qml_cpp_bridge.hpp"
#include "network.hpp"
#include "cfg_file.hpp"
#include "shared/tcp_socket.hpp"
#include "list_model.hpp"

// Qt include.
#include <QStringListModel>
#include <QSortFilterProxyModel>


namespace Stock {

//
// QmlCppSignalsPrivate
//

class QmlCppBridgePrivate {
public:
	QmlCppBridgePrivate( const QString & cfgFileName, QmlCppBridge * parent )
		:	m_cfgFileName( cfgFileName )
		,	m_net( Q_NULLPTR )
		,	m_codesModel( Q_NULLPTR )
		,	m_placesModel( Q_NULLPTR )
		,	m_sortCodesModel( Q_NULLPTR )
		,	m_sortPlacesModel( Q_NULLPTR )
		,	m_searchModel( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Cfg file name.
	QString m_cfgFileName;
	//! Network.
	Network * m_net;
	//! Codes model.
	QStringListModel * m_codesModel;
	//! Places model.
	QStringListModel * m_placesModel;
	//! Codes sort model.
	QSortFilterProxyModel * m_sortCodesModel;
	//! Places sort model.
	QSortFilterProxyModel * m_sortPlacesModel;
	//! Search model.
	ListModel * m_searchModel;
	//! Parent.
	QmlCppBridge * q;
}; // class QmlCppSignalsPrivate

void
QmlCppBridgePrivate::init()
{
	m_net = new Network( q );

	m_codesModel = new QStringListModel( q );
	m_placesModel = new QStringListModel( q );

	m_sortCodesModel = new QSortFilterProxyModel( q );
	m_sortCodesModel->setSourceModel( m_codesModel );

	m_sortPlacesModel = new QSortFilterProxyModel( q );
	m_sortPlacesModel->setSourceModel( m_placesModel );

	m_searchModel = new ListModel( q );
}


//
// QmlCppSignals
//

QmlCppBridge::QmlCppBridge( const QString & configFileName )
	:	d( new QmlCppBridgePrivate( configFileName, this ) )
{
	d->init();

	connect( this, &QmlCppBridge::connectRequest,
		this, &QmlCppBridge::connectRequested, Qt::QueuedConnection );
	connect( d->m_net, &Network::disconnected,
		this, &QmlCppBridge::networkDisconnected, Qt::QueuedConnection );
	connect( d->m_net, &Network::error,
		this, &QmlCppBridge::serverError, Qt::QueuedConnection );
	connect( this, &QmlCppBridge::disconnectRequest,
		this, &QmlCppBridge::disconnectRequested, Qt::QueuedConnection );
	connect( d->m_net, &Network::connected,
		this, &QmlCppBridge::connected, Qt::QueuedConnection );
	connect( this, &QmlCppBridge::putProduct,
		this, &QmlCppBridge::putProductRequested, Qt::QueuedConnection );
	connect( this, &QmlCppBridge::takeProduct,
		this, &QmlCppBridge::takeProductRequested, Qt::QueuedConnection );
	connect( d->m_net, &Network::opOk,
		this, &QmlCppBridge::opOk, Qt::QueuedConnection );
	connect( d->m_net, &Network::opFailed,
		this, &QmlCppBridge::opFailed, Qt::QueuedConnection );
	connect( this, &QmlCppBridge::search,
		this, &QmlCppBridge::searchRequested, Qt::QueuedConnection );
	connect( d->m_net->socket(), &TcpSocket::listOfProducts,
		this, &QmlCppBridge::listOfProductsReceived, Qt::QueuedConnection );
}

QmlCppBridge::~QmlCppBridge()
{
}

QSortFilterProxyModel *
QmlCppBridge::codesModel() const
{
	return d->m_sortCodesModel;
}

QSortFilterProxyModel *
QmlCppBridge::placesModel() const
{
	return d->m_sortPlacesModel;
}

ListModel *
QmlCppBridge::searchModel() const
{
	return d->m_searchModel;
}

void
QmlCppBridge::connectRequested( const QString & pwd, quint16 port )
{
	d->m_net->setPassword( pwd );
	d->m_net->setUdpPort( port );
	d->m_net->establishConnection();

	CfgFile::write( Stock::CfgFile::fileName(), pwd, port );
}

void
QmlCppBridge::networkDisconnected( bool requestedByUser )
{
	emit disconnected( requestedByUser ? QString() :
		tr( "Something went wrong on the server. "
			"Check your password and try to reconnect." ) );
}

void
QmlCppBridge::connected( const QStringList & codes, const QStringList & places )
{
	d->m_codesModel->setStringList( codes );
	d->m_placesModel->setStringList( places );

	d->m_sortCodesModel->sort( 0 );
	d->m_sortPlacesModel->sort( 0 );

	emit connectionEstablished();
}

void
QmlCppBridge::disconnectRequested()
{
	d->m_net->disconnectNetwork( true );
}

void
QmlCppBridge::serverError()
{
	networkDisconnected( false );
}

void
QmlCppBridge::putProductRequested( const QString & code, const QString & place, long long count )
{
	Messages::AddProduct msg;
	msg.set_code( code );
	msg.set_place( place );
	msg.set_count( count );
	msg.set_secret( d->m_net->password() );

	d->m_net->socket()->sendAddProduct( msg );
}

void
QmlCppBridge::takeProductRequested( const QString & code, const QString & place, long long count )
{
	Messages::AddProduct msg;
	msg.set_code( code );
	msg.set_place( place );
	msg.set_count( -count );
	msg.set_secret( d->m_net->password() );

	d->m_net->socket()->sendAddProduct( msg );
}

void
QmlCppBridge::searchRequested( const QString & code, const QString & place )
{
	Messages::GiveListOfProducts msg;
	msg.set_code( code );
	msg.set_place( place );
	msg.set_secret( d->m_net->password() );

	d->m_net->socket()->sendGiveListOfProducts( msg );
}

void
QmlCppBridge::listOfProductsReceived( const Stock::Messages::ListOfProducts & msg )
{
	d->m_searchModel->setVectorData( msg.product() );

	emit listReceived();
}

} /* namespace Stock */
