
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

// Qt include.
#include <QObject>

// Stock include.
#include "db_signals.hpp"


namespace Stock {

//
// DbSignals
//

DbSignals::DbSignals( QObject * parent )
	:	QObject( parent )
{
}

DbSignals::~DbSignals()
{
}

void
DbSignals::emitProductDeleted( const QString & code )
{
	emit productDeleted( code );
}

void
DbSignals::emitProductChanged( const QString & code, const QString & place,
	quint64 count, const QString & desc )
{
	emit productChanged( code, place, count, desc );
}

void
DbSignals::emitCodeChanged( const QString & newCode, const QString & oldCode )
{
	emit codeChanged( newCode, oldCode );
}

void
DbSignals::emitPlaceRenamed( const QString & newName, const QString & oldName )
{
	emit placeRenamed( newName, oldName );
}

void
DbSignals::emitPlaceDeleted( const QString & place )
{
	emit placeDeleted( place );
}

} /* namespace Stock */
