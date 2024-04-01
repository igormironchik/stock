
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "by_product_sort_model.hpp"


namespace Stock {

//
// ByProductSortModelPrivate
//

class ByProductSortModelPrivate
	:	public SortFilterModelPrivate
{
public:
	ByProductSortModelPrivate( ByProductSortModel * parent )
		:	SortFilterModelPrivate( parent )
	{
	}

	//! \return Is place accepted?
	bool acceptPlace( int sourceRow,
		const QModelIndex & sourceParent ) const;

	inline ByProductSortModel * q_func() const
	{
		return static_cast< ByProductSortModel* > ( q );
	}
}; // class ByProductSortModelPrivate

bool
ByProductSortModelPrivate::acceptPlace( int sourceRow,
	const QModelIndex & sourceParent ) const
{
	const QModelIndex place = q_func()->sourceModel()->index( sourceRow, 0,
		sourceParent );

	return place.data().toString().contains( m_place );
}


//
// ByProductSortModel
//

ByProductSortModel::ByProductSortModel( QObject * parent )
	:	SortFilterModel( new ByProductSortModelPrivate( this ), parent )
{
}

ByProductSortModel::~ByProductSortModel()
{
}

bool
ByProductSortModel::filterAcceptsRow( int sourceRow,
	const QModelIndex & sourceParent ) const
{
	const ByProductSortModelPrivate * dd = d_func();

	if( !sourceParent.isValid() )
	{
		const QModelIndex code = sourceModel()->index( sourceRow, 0,
			sourceParent );
		const QModelIndex desc = sourceModel()->index( sourceRow, 2,
			sourceParent );

		const auto rowsCount = sourceModel()->rowCount( code );

		bool empty = true;

		if( !d->m_place.isEmpty() )
		{
			for( auto i = 0; i < rowsCount; ++i )
			{
				if( dd->acceptPlace( i, code ) )
				{
					empty = false;

					break;
				}
			}
		}
		else
			empty = false;

		return ( code.data().toString().contains( dd->m_code ) &&
			desc.data().toString().toLower().contains( dd->m_desc.toLower() ) && !empty );
	}
	else
		return dd->acceptPlace( sourceRow, sourceParent );
}

} /* namespace Stock */
