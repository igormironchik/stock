
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
#include "by_place_sort_model.hpp"


namespace Stock {

//
// ByPlaceSortModelPrivate
//

class ByPlaceSortModelPrivate {
public:
	ByPlaceSortModelPrivate( ByPlaceSortModel * parent )
		:	q( parent )
	{
	}

	//! \return Is product accepted?
	bool acceptProduct( int sourceRow,
		const QModelIndex & sourceParent ) const;

	//! Code.
	QString m_code;
	//! Place.
	QString m_place;
	//! Description.
	QString m_desc;
	//! Parent.
	ByPlaceSortModel * q;
}; // class ByPlaceSortModelPrivate

bool
ByPlaceSortModelPrivate::acceptProduct( int sourceRow,
	const QModelIndex & sourceParent ) const
{
	const QModelIndex code = q->sourceModel()->index( sourceRow, 0,
		sourceParent );
	const QModelIndex desc = q->sourceModel()->index( sourceRow, 2,
		sourceParent );

	return ( code.data().toString().contains( m_code ) &&
		desc.data().toString().contains( m_desc ) );
}


//
// ByPlaceSortModel
//

ByPlaceSortModel::ByPlaceSortModel( QObject * parent )
	:	QSortFilterProxyModel( parent )
	,	d( new ByPlaceSortModelPrivate( this ) )
{
}

ByPlaceSortModel::~ByPlaceSortModel()
{
}

void
ByPlaceSortModel::setFilterData( const QString & code, const QString & place,
	const QString & desc )
{
	d->m_code = code;
	d->m_place = place;
	d->m_desc = desc;

	invalidateFilter();
}

bool
ByPlaceSortModel::filterAcceptsRow( int sourceRow,
	const QModelIndex & sourceParent ) const
{
	if( !sourceParent.isValid() )
	{
		const QModelIndex place = sourceModel()->index( sourceRow, 0,
			sourceParent );

		const auto rowsCount = sourceModel()->rowCount( place );

		bool empty = true;

		for( auto i = 0; i < rowsCount; ++i )
		{
			if( d->acceptProduct( i, place ) )
			{
				empty = false;

				break;
			}
		}

		return ( place.data().toString().contains( d->m_place )  && !empty );
	}
	else
		return d->acceptProduct( sourceRow, sourceParent );
}

} /* namespace Stock */
