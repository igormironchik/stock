
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018-2020 Igor Mironchik

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
		return reinterpret_cast< ByPlaceSortModelPrivate* > ( d.data() );
	}
}; // class ByPlaceSortModel

} /* namespace Stock */

#endif // STOCK_BY_PLACE_SORT_MODEL_HPP_INCLUDED
