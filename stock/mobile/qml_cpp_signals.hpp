
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

#ifndef STOCK_QML_CPP_SIGNALS_HPP_INCLUDED
#define STOCK_QML_CPP_SIGNALS_HPP_INCLUDED

// Qt include.
#include <QObject>
#include <QScopedPointer>


namespace Stock {

//
// QmlCppSignals
//

class QmlCppSignalsPrivate;

//! Bridge between QML and C++.
class QmlCppSignals Q_DECL_FINAL
	:	public QObject
{
	Q_OBJECT

signals:
	//! Connect request.
	void connectRequest( const QString & pwd );
	//! Connection established.
	void connectionEstablished();

public:
	explicit QmlCppSignals( const QString & configFileName );
	virtual ~QmlCppSignals();

private slots:
	//! Connect requested.
	void connectRequested( const QString & pwd );

private:
	friend class QmlCppSignalsPrivate;

	Q_DISABLE_COPY( QmlCppSignals )

	QScopedPointer< QmlCppSignalsPrivate > d;
}; // class QmlCppSignals

} /* namespace Stock */

#endif // STOCK_QML_CPP_SIGNALS_HPP_INCLUDED
