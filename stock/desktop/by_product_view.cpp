
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
#include "rename.hpp"
#include "db.hpp"
#include "db_signals.hpp"

// Qt include.
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>


namespace Stock {

//
// ByProductViewPrivate
//

class ByProductViewPrivate {
public:
	ByProductViewPrivate( ByProductView * parent )
		:	m_model( Q_NULLPTR )
		,	m_filter( Q_NULLPTR )
		,	m_sigs( Q_NULLPTR )
		,	m_db( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Index of context menu.
	QModelIndex m_index;
	//! Source model.
	ByProductModel * m_model;
	//! Filter model.
	ByProductSortModel * m_filter;
	//! DB signals.
	DbSignals * m_sigs;
	//! DB.
	Db * m_db;
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

void
ByProductView::setDb( DbSignals * sigs, Db * db )
{
	d->m_db = db;
	d->m_sigs = sigs;
}

void
ByProductView::contextMenuEvent( QContextMenuEvent * e )
{
	d->m_index = d->m_filter->mapToSource( indexAt( e->pos() ) );

	if( d->m_index.isValid() )
	{
		if( !d->m_index.parent().isValid() )
		{
			QMenu menu;
			menu.addAction( QIcon( ":/img/edit-rename_22x22.png" ),
				tr( "Change Product's Code" ), this, &ByProductView::changeCode );

			menu.exec( e->globalPos() );

			e->accept();
		}
		else
			e->ignore();
	}
	else
		e->ignore();
}

void
ByProductView::changeCode()
{
	if( d->m_db && d->m_sigs && d->m_index.isValid() )
	{
		const auto oldCode = d->m_model->data( d->m_model->index( d->m_index.row(), 0 ) )
			.toString();

		RenameDlg dlg( oldCode, d->m_model->codes(), this );
		dlg.setWindowTitle( tr( "Change Product's Code..." ) );

		if( dlg.exec() == QDialog::Accepted )
		{
			DbResult res = d->m_db->changeCode( oldCode, dlg.renamed() );

			if( !res.m_ok )
			{
				QMessageBox::critical( this, tr( "Error in the database..." ),
					tr( "Unable to change code of the product.\n\n%1" )
						.arg( res.m_error ) );
			}
			else
				d->m_sigs->emitCodeChanged( dlg.renamed(), oldCode );
		}
	}
}

} /* namespace Stock */
