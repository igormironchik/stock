
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
