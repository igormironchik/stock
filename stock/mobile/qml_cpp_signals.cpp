
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
#include "qml_cpp_signals.hpp"
#include "network.hpp"


namespace Stock {

//
// QmlCppSignalsPrivate
//

class QmlCppSignalsPrivate {
public:
	QmlCppSignalsPrivate( const QString & cfgFileName, QmlCppSignals * parent )
		:	m_cfgFileName( cfgFileName )
		,	m_net( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Cfg file name.
	QString m_cfgFileName;
	//! Network.
	Network * m_net;
	//! Parent.
	QmlCppSignals * q;
}; // class QmlCppSignalsPrivate

void
QmlCppSignalsPrivate::init()
{
	m_net = new Network( q );
}


//
// QmlCppSignals
//

QmlCppSignals::QmlCppSignals( const QString & configFileName )
	:	d( new QmlCppSignalsPrivate( configFileName, this ) )
{
	d->init();

	connect( this, &QmlCppSignals::connectRequest,
		this, &QmlCppSignals::connectRequested, Qt::QueuedConnection );
}

QmlCppSignals::~QmlCppSignals()
{
}

void
QmlCppSignals::connectRequested( const QString & pwd )
{
	emit connectionEstablished();
}

} /* namespace Stock */
