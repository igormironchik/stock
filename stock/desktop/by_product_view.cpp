
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
#include "by_product_view.hpp"
#include "by_product_model.hpp"
#include "by_product_sort_model.hpp"


namespace Stock {

//
// ByProductViewPrivate
//

class ByProductViewPrivate {
public:
	ByProductViewPrivate( ByProductView * parent )
		:	m_model( Q_NULLPTR )
		,	m_filter( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Source model.
	ByProductModel * m_model;
	//! Filter model.
	ByProductSortModel * m_filter;
	//! Parent.
	ByProductView * q;
}; // class ByProductViewPrivate

void
ByProductViewPrivate::init()
{

}


//
// ByProductView
//

ByProductView::ByProductView( QWidget * parent )
	:	QTreeView( parent )
	,	d( new ByProductViewPrivate( this ) )
{
	d->init();
}

ByProductView::~ByProductView()
{
}

void
ByProductView::setFilterModel( ByProductSortModel * filter )
{
	d->m_filter = filter;

	setModel( d->m_filter );
}

void
ByProductView::setSourceModel( ByProductModel * model )
{
	d->m_model = model;
}

} /* namespace Stock */
