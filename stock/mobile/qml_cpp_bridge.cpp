
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
#include "qml_cpp_bridge.hpp"
#include "network.hpp"
#include "cfg_file.hpp"

// Qt include.
#include <QStringListModel>


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
	//! Parent.
	QmlCppBridge * q;
}; // class QmlCppSignalsPrivate

void
QmlCppBridgePrivate::init()
{
	m_net = new Network( q );
	m_codesModel = new QStringListModel( q );
	m_placesModel = new QStringListModel( q );
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
}

QmlCppBridge::~QmlCppBridge()
{
}

QStringListModel *
QmlCppBridge::codesModel() const
{
	return d->m_codesModel;
}

QStringListModel *
QmlCppBridge::placesModel() const
{
	return d->m_placesModel;
}

void
QmlCppBridge::connectRequested( const QString & pwd )
{
	d->m_net->setPassword( pwd );
	d->m_net->establishConnection();

	CfgFile::write( Stock::CfgFile::fileName(), pwd );
}

void
QmlCppBridge::networkDisconnected( bool requestedByUser )
{
	emit disconnected( requestedByUser ? QString() :
		tr( "Network error occured. Try to reconnect." ) );
}

void
QmlCppBridge::connected( const QStringList & codes, const QStringList & places )
{
	d->m_codesModel->setStringList( codes );
	d->m_placesModel->setStringList( places );

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
	emit disconnected( tr( "Something went wrong on the server. "
		"Check your password and try to reconnect." ) );
}

} /* namespace Stock */
