
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
		desc.data().toString().toLower().contains( m_desc.toLower() ) );
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
