
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
#include "by_place_view.hpp"
#include "by_place_model.hpp"


namespace Stock {

//
// ByPlaceViewPrivate
//

class ByPlaceViewPrivate {
public:
	ByPlaceViewPrivate( ByPlaceView * parent )
		:	m_model( Q_NULLPTR )
		,	m_filter( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Source model.
	ByPlaceModel * m_model;
	//! Filter model.
	ByPlaceSortModel * m_filter;
	//! Parent.
	ByPlaceView * q;
}; // class ByPlaceViewPrivate

void
ByPlaceViewPrivate::init()
{

}


//
// ByPlaceView
//

ByPlaceView::ByPlaceView( QWidget * parent )
	:	QTreeView( parent )
	,	d( new ByPlaceViewPrivate( this ) )
{
	d->init();
}

ByPlaceView::~ByPlaceView()
{
}

void
ByPlaceView::setModels( ByPlaceModel * model,
	ByPlaceSortModel * filter )
{
	d->m_model = model;
	d->m_filter = filter;
}

} /* namespace Stock */
