
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

#ifndef STOCK_NETWORK_HPP_INCLUDED
#define STOCK_NETWORK_HPP_INCLUDED

// Qt include.
#include <QObject>
#include <QScopedPointer>


namespace Stock {

class QmlCppSignals;

//
// Network
//

class NetworkPrivate;

//! Network communications.
class Network Q_DECL_FINAL
	:	public QObject
{
	Q_OBJECT

public:
	explicit Network( QmlCppSignals * sigs );
	virtual ~Network();

private:
	friend class NetworkPrivate;

	Q_DISABLE_COPY( Network )

	QScopedPointer< NetworkPrivate > d;
}; // class Network

} /* namespace Stock */

#endif // STOCK_NETWORK_HPP_INCLUDED
