
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
#include "by_place_model.hpp"
#include "db.hpp"
#include "db_signals.hpp"


namespace Stock {

//
// ByPlaceModelPrivate
//

class ByPlaceModelPrivate {
public:
	ByPlaceModelPrivate( MainWindow * win, Db * db,
		DbSignals * sigs, ByPlaceModel * parent )
		:	m_win( win )
		,	m_db( db )
		,	m_sigs( sigs )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Main window.
	MainWindow * m_win;
	//! DB.
	Db * m_db;
	//! DB signals.
	DbSignals * m_sigs;
	//! Parent.
	ByPlaceModel * q;
}; // class ByPlaceModelPrivate

void
ByPlaceModelPrivate::init()
{

}


//
// ByPlaceModel
//

ByPlaceModel::ByPlaceModel( MainWindow * win, Db * db,
	DbSignals * sigs, QObject * parent )
	:	QAbstractItemModel( parent )
	,	d( new ByPlaceModelPrivate( win, db, sigs, this ) )
{
	d->init();
}

ByPlaceModel::~ByPlaceModel()
{
}

QStringList
ByPlaceModel::places() const
{
	return QStringList();
}

int
ByPlaceModel::columnCount( const QModelIndex & parent ) const
{
	return 0;
}

QVariant
ByPlaceModel::data( const QModelIndex & index, int role ) const
{
	return QVariant();
}

Qt::ItemFlags
ByPlaceModel::flags( const QModelIndex & index ) const
{
	return Qt::NoItemFlags;
}

bool
ByPlaceModel::hasChildren( const QModelIndex & parent ) const
{
	return false;
}

QVariant
ByPlaceModel::headerData( int section, Qt::Orientation orientation,
	int role ) const
{
	return QVariant();
}

QModelIndex
ByPlaceModel::index( int row, int column,
	const QModelIndex & parent ) const
{
	return QModelIndex();
}

QModelIndex
ByPlaceModel::parent( const QModelIndex & index ) const
{
	return QModelIndex();
}

int
ByPlaceModel::rowCount( const QModelIndex & parent ) const
{
	return 0;
}

bool
ByPlaceModel::setData( const QModelIndex & index, const QVariant & value,
	int role )
{
	return false;
}

void
ByPlaceModel::productDeleted( const QString & code )
{

}

void
ByPlaceModel::productChanged( const QString & code, const QString & place,
	quint64 count, const QString & desc )
{

}

} /* namespace Stock */
