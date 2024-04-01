
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_BY_PRODUCT_SORT_MODEL_HPP_INCLUDED
#define STOCK_BY_PRODUCT_SORT_MODEL_HPP_INCLUDED

// Stock include.
#include "sort_filter_base_model.hpp"


namespace Stock {

//
// ByProductSortModel
//

class ByProductSortModelPrivate;

//! Sort-filter model for "By Product" mode.
class ByProductSortModel Q_DECL_FINAL
	:	public SortFilterModel
{
	Q_OBJECT

public:
	explicit ByProductSortModel( QObject * parent );
	virtual ~ByProductSortModel();

protected:
	bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
		Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY( ByProductSortModel )

	inline ByProductSortModelPrivate * d_func() const
	{
		return reinterpret_cast< ByProductSortModelPrivate* > ( d.get() );
	}
}; // class ByProductSortModel

} /* namespace Stock */

#endif // STOCK_BY_PRODUCT_SORT_MODEL_HPP_INCLUDED
