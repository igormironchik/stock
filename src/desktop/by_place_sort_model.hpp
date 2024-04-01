
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_BY_PLACE_SORT_MODEL_HPP_INCLUDED
#define STOCK_BY_PLACE_SORT_MODEL_HPP_INCLUDED

// Stock include.
#include "sort_filter_base_model.hpp"


namespace Stock {

//
// ByPlaceSortModel
//

class ByPlaceSortModelPrivate;

//! Sort-filter model for "By Product" mode.
class ByPlaceSortModel Q_DECL_FINAL
	:	public SortFilterModel
{
	Q_OBJECT

public:
	explicit ByPlaceSortModel( QObject * parent );
	virtual ~ByPlaceSortModel();

protected:
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
		Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY( ByPlaceSortModel )

	inline ByPlaceSortModelPrivate * d_func() const
	{
		return reinterpret_cast< ByPlaceSortModelPrivate* > ( d.get() );
	}
}; // class ByPlaceSortModel

} /* namespace Stock */

#endif // STOCK_BY_PLACE_SORT_MODEL_HPP_INCLUDED
