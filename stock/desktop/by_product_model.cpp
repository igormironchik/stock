
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
#include "by_product_model.hpp"
#include "db.hpp"
#include "db_signals.hpp"


namespace Stock {

//
// ByProductModelPrivate
//

class ByProductModelPrivate {
public:
	ByProductModelPrivate( Db * db, DbSignals * sigs, ByProductModel * parent )
		:	m_db( db )
		,	m_sigs( sigs )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! DB.
	Db * m_db;
	//! DB signals.
	DbSignals * m_sigs;
	//! Parent.
	ByProductModel * q;
}; // class ByProductModelPrivate

void
ByProductModelPrivate::init()
{

}


//
// ByProductModel
//

ByProductModel::ByProductModel( Db * db, DbSignals * sigs, QObject * parent )
	:	QAbstractItemModel( parent )
	,	d( new ByProductModelPrivate( db, sigs, this ) )
{
	d->init();
}

ByProductModel::~ByProductModel()
{
}

int
ByProductModel::columnCount( const QModelIndex & parent ) const
{
	return 0;
}

QVariant
ByProductModel::data( const QModelIndex & index, int role ) const
{
	return QVariant();
}

Qt::ItemFlags
ByProductModel::flags( const QModelIndex & index ) const
{
	return Qt::NoItemFlags;
}

bool
ByProductModel::hasChildren( const QModelIndex & parent ) const
{
	return false;
}

QVariant
ByProductModel::headerData( int section, Qt::Orientation orientation,
	int role ) const
{
	return QVariant();
}

QModelIndex
ByProductModel::index( int row, int column, const QModelIndex & parent ) const
{
	return QModelIndex();
}

QModelIndex
ByProductModel::parent( const QModelIndex & index ) const
{
	return QModelIndex();
}

int
ByProductModel::rowCount( const QModelIndex & parent ) const
{
	return 0;
}

bool
ByProductModel::setData( const QModelIndex & index, const QVariant & value,
	int role )
{
	return false;
}

} /* namespace Stock */
