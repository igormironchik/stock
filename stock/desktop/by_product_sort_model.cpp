
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
#include "by_product_sort_model.hpp"


namespace Stock {

//
// ByProductSortModelPrivate
//

class ByProductSortModelPrivate {
public:
	ByProductSortModelPrivate( ByProductSortModel * parent )
		:	q( parent )
	{
	}

	//! Code.
	QString m_code;
	//! Place.
	QString m_place;
	//! Description.
	QString m_desc;
	//! Parent.
	ByProductSortModel * q;
}; // class ByProductSortModelPrivate


//
// ByProductSortModel
//

ByProductSortModel::ByProductSortModel( QObject * parent )
	:	QSortFilterProxyModel( parent )
	,	d( new ByProductSortModelPrivate( this ) )
{
}

ByProductSortModel::~ByProductSortModel()
{
}

void
ByProductSortModel::setFilterData( const QString & code, const QString & place,
	const QString & desc )
{
	d->m_code = code;
	d->m_place = place;
	d->m_desc = desc;

	invalidateFilter();
}

bool
ByProductSortModel::filterAcceptsRow( int sourceRow,
	const QModelIndex & sourceParent ) const
{
	if( !sourceParent.isValid() )
	{
		const QModelIndex code = sourceModel()->index( sourceRow, 0,
			sourceParent );
		const QModelIndex desc = sourceModel()->index( sourceRow, 2,
			sourceParent );

		return ( code.data().toString().contains( d->m_code ) &&
			desc.data().toString().contains( d->m_desc ) );
	}
	else
	{
		const QModelIndex place = sourceModel()->index( sourceRow, 0,
			sourceParent );

		return place.data().toString().contains( d->m_place );
	}
}

} /* namespace Stock */
