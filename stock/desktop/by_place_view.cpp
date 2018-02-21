
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
#include "by_place_sort_model.hpp"
#include "by_product_model.hpp"

// Qt include.
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>


namespace Stock {

//
// ByPlaceViewPrivate
//

class ByPlaceViewPrivate {
public:
	ByPlaceViewPrivate( ByPlaceView * parent )
		:	m_model( Q_NULLPTR )
		,	m_filter( Q_NULLPTR )
		,	m_auxModel( Q_NULLPTR )
		,	m_sigs( Q_NULLPTR )
		,	m_db( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Index of context menu.
	QModelIndex m_index;
	//! Parent index of context menu.
	QModelIndex m_parentIndex;
	//! Source model.
	ByPlaceModel * m_model;
	//! Filter model.
	ByPlaceSortModel * m_filter;
	//! Auxiliary model.
	ByProductModel * m_auxModel;
	//! DB signals.
	DbSignals * m_sigs;
	//! DB.
	Db * m_db;
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
ByPlaceView::setFilterModel( ByPlaceSortModel * filter )
{
	d->m_filter = filter;

	setModel( d->m_filter );
}

void
ByPlaceView::setSourceModel( ByPlaceModel * model )
{
	d->m_model = model;
}

void
ByPlaceView::setAuxiliaryModel( ByProductModel * model )
{
	d->m_auxModel = model;
}

void
ByPlaceView::setDb( DbSignals * sigs, Db * db )
{
	d->m_db = db;
	d->m_sigs = sigs;
}

void
ByPlaceView::contextMenuEvent( QContextMenuEvent * e )
{
	d->m_index = d->m_filter->mapToSource( indexAt( e->pos() ) );
	d->m_parentIndex = d->m_index.parent();

	if( d->m_index.isValid() )
	{
		QMenu menu;
		menu.addAction( QIcon( ":/img/flag-blue_22x22.png" ),
			tr( "Rename Place" ), this, &ByPlaceView::renamePlace );

		if( d->m_index.parent().isValid() )
		{
			menu.addAction( QIcon( ":/img/view-barcode_22x22.png" ),
				tr( "Change Code" ), this, &ByPlaceView::changeCode );
			menu.addAction( QIcon( ":/img/document-edit_22x22.png" ),
				tr( "Change Description" ), this, &ByPlaceView::changeDesc );
		}

		if( !d->m_parentIndex.isValid() && !d->m_model->hasChildren( d->m_index ) )
		{
			menu.addSeparator();
			menu.addAction( QIcon( ":/img/edit-delete_22x22.png" ),
				tr( "Delete Place" ), this, &ByPlaceView::deletePlace );
		}

		menu.exec( e->globalPos() );

		e->accept();
	}
	else
		e->ignore();
}

void
ByPlaceView::changeCode()
{

}

void
ByPlaceView::changeDesc()
{

}

void
ByPlaceView::renamePlace()
{

}

void
ByPlaceView::deletePlace()
{

}

} /* namespace Stock */
