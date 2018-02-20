
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

class ByPlaceSortModelPrivate
	:	public SortFilterModelPrivate
{
public:
	ByPlaceSortModelPrivate( ByPlaceSortModel * parent )
		:	SortFilterModelPrivate( parent )
	{
	}

	//! \return Is product accepted?
	bool acceptProduct( int sourceRow,
		const QModelIndex & sourceParent ) const;

	inline ByPlaceSortModel * q_func() const
	{
		return static_cast< ByPlaceSortModel* > ( q );
	}
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
	:	SortFilterModel( new ByPlaceSortModelPrivate( this ), parent )
{
}

ByPlaceSortModel::~ByPlaceSortModel()
{
}

bool
ByPlaceSortModel::filterAcceptsRow( int sourceRow,
	const QModelIndex & sourceParent ) const
{
	const ByPlaceSortModelPrivate * dd = d_func();

	if( !sourceParent.isValid() )
	{
		const QModelIndex place = sourceModel()->index( sourceRow, 0,
			sourceParent );

		const auto rowsCount = sourceModel()->rowCount( place );

		bool empty = true;

		if( !d->m_code.isEmpty() || !d->m_desc.isEmpty() )
		{
			for( auto i = 0; i < rowsCount; ++i )
			{
				if( dd->acceptProduct( i, place ) )
				{
					empty = false;

					break;
				}
			}
		}
		else
			empty = false;

		return ( place.data().toString().contains( d->m_place )  && !empty );
	}
	else
		return dd->acceptProduct( sourceRow, sourceParent );
}

} /* namespace Stock */
