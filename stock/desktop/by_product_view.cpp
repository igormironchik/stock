
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
#include "edit_desc.hpp"
#include "by_place_model.hpp"
#include "amount.hpp"
#include "word_wrap_delegate.hpp"

// Qt include.
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QHeaderView>


namespace Stock {

//
// ByProductViewPrivate
//

class ByProductViewPrivate {
public:
	ByProductViewPrivate( ByProductView * parent )
		:	m_model( Q_NULLPTR )
		,	m_filter( Q_NULLPTR )
		,	m_auxModel( Q_NULLPTR )
		,	m_sigs( Q_NULLPTR )
		,	m_db( Q_NULLPTR )
		,	m_delegate( nullptr )
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
	ByProductModel * m_model;
	//! Filter model.
	ByProductSortModel * m_filter;
	//! Auxiliary model.
	ByPlaceModel * m_auxModel;
	//! DB signals.
	DbSignals * m_sigs;
	//! DB.
	Db * m_db;
	//! Delegate for description.
	WordWrapItemDelegate * m_delegate;
	//! Parent.
	ByProductView * q;
}; // class ByProductViewPrivate

void
ByProductViewPrivate::init()
{
	m_delegate = new WordWrapItemDelegate( q );
	q->setItemDelegateForColumn( m_model->columnCount() - 1, m_delegate );
}


//
// ByProductView
//

ByProductView::ByProductView( QWidget * parent )
	:	QTreeView( parent )
	,	d( new ByProductViewPrivate( this ) )
{
	d->init();

	connect( header(), &QHeaderView::sectionResized,
		this, &ByProductView::sectionResized );
}

ByProductView::~ByProductView()
{
}

void
ByProductView::sectionResized( int section, int, int )
{
	if( section == d->m_model->columnCount() - 1 )
	{
		for( int i = 0; i < d->m_model->rowCount(); ++i )
			emit d->m_delegate->sizeHintChanged( d->m_model->index( i, section ) );
	}
}

void
ByProductView::setFilterModel( ByProductSortModel * filter )
{
	d->m_filter = filter;

	setModel( d->m_filter );

	header()->resizeSections( QHeaderView::ResizeToContents );
}

void
ByProductView::setSourceModel( ByProductModel * model )
{
	d->m_model = model;
}

void
ByProductView::setAuxiliaryModel( ByPlaceModel * model )
{
	d->m_auxModel = model;
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
	d->m_parentIndex = d->m_index.parent();

	if( d->m_index.isValid() )
	{
		QMenu menu;
		menu.addAction( QIcon( ":/img/view-barcode_22x22.png" ),
			tr( "Change Code" ), this, &ByProductView::changeCode );
		menu.addAction( QIcon( ":/img/document-edit_22x22.png" ),
			tr( "Change Description" ), this, &ByProductView::changeDesc );

		if( d->m_index.parent().isValid() )
		{
			menu.addAction( QIcon( ":/img/flag-blue_22x22.png" ),
				tr( "Rename Place" ), this, &ByProductView::renamePlace );
			menu.addAction( QIcon( ":/img/list-add_22x22.png" ),
				tr( "Change Amount" ), this, &ByProductView::changeAmount );
		}

		if( !d->m_parentIndex.isValid() && !d->m_model->hasChildren( d->m_index ) )
		{
			menu.addSeparator();
			menu.addAction( QIcon( ":/img/edit-delete_22x22.png" ),
				tr( "Delete Product" ), this, &ByProductView::deleteProduct );
		}

		menu.exec( e->globalPos() );

		e->accept();
	}
	else
		e->ignore();
}

void
ByProductView::changeCode()
{
	QModelIndex index = d->m_parentIndex;

	if( !index.isValid() )
		index = d->m_index;

	if( d->m_db && d->m_sigs && index.isValid() )
	{
		const auto oldCode = d->m_model->data( d->m_model->index( index.row(), 0 ) )
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

void
ByProductView::changeDesc()
{
	QModelIndex index = d->m_parentIndex;

	if( !index.isValid() )
		index = d->m_index;

	if( d->m_db && d->m_sigs && index.isValid() )
	{
		const auto oldDesc = d->m_model->data( d->m_model->index( index.row(), 2 ) )
			.toString();
		const auto code = d->m_model->data( d->m_model->index( index.row(), 0 ) )
			.toString();

		EditDescDlg dlg( oldDesc, this );

		if( dlg.exec() == QDialog::Accepted )
		{
			DbResult res = d->m_db->changeProduct( { code, QString(), 0, dlg.text() } );

			if( !res.m_ok )
			{
				QMessageBox::critical( this, tr( "Error in the database..." ),
					tr( "Unable to change description of the product.\n\n%1" )
						.arg( res.m_error ) );
			}
			else
				d->m_sigs->emitProductChanged( code, QString(), 0, dlg.text() );
		}
	}
}

void
ByProductView::renamePlace()
{
	if( d->m_db && d->m_sigs && d->m_index.isValid() && d->m_parentIndex.isValid() )
	{
		const auto oldPlace = d->m_model->data( d->m_model->index( d->m_index.row(), 0,
			d->m_parentIndex ) ).toString();

		RenameDlg dlg( oldPlace, d->m_auxModel->places(), this );
		dlg.setWindowTitle( tr( "Rename Place..." ) );

		if( dlg.exec() == QDialog::Accepted )
		{
			DbResult res = d->m_db->renamePlace( oldPlace, dlg.renamed() );

			if( !res.m_ok )
			{
				QMessageBox::critical( this, tr( "Error in the database..." ),
					tr( "Unable to rename place.\n\n%1" )
						.arg( res.m_error ) );
			}
			else
				d->m_sigs->emitPlaceRenamed( dlg.renamed(), oldPlace );
		}
	}
}

void
ByProductView::deleteProduct()
{
	if( d->m_db && d->m_sigs && d->m_index.isValid() )
	{
		const auto code = d->m_model->data( d->m_model->index( d->m_index.row(), 0 ) )
			.toString();

		const auto res = QMessageBox::question( this, tr( "Deletion of Product..." ),
			tr( "You are about to completely remove product from the database.\n\n"
				"Are you sure that you want to delete product \"%1\"?" )
					.arg( code ), QMessageBox::Yes,
				QMessageBox::No | QMessageBox::Default | QMessageBox::Escape );

		if( res == QMessageBox::Yes )
		{
			DbResult res = d->m_db->deleteProduct( code );

			if( !res.m_ok )
			{
				QMessageBox::critical( this, tr( "Error in the database..." ),
					tr( "Unable to remove product.\n\n%1" )
						.arg( res.m_error ) );
			}
			else
				d->m_sigs->emitProductDeleted( code );
		}
	}
}

void
ByProductView::changeAmount()
{
	if( d->m_db && d->m_sigs && d->m_index.isValid() && d->m_parentIndex.isValid() )
	{
		const auto code = d->m_model->data( d->m_model->index( d->m_parentIndex.row(), 0 ) )
			.toString();
		const auto desc = d->m_model->data( d->m_model->index( d->m_parentIndex.row(), 2 ) )
			.toString();
		const auto oldAmount = d->m_model->data( d->m_model->index( d->m_index.row(), 1,
			d->m_parentIndex ) ).toULongLong();
		const auto place = d->m_model->data( d->m_model->index( d->m_index.row(), 0,
			d->m_parentIndex ) ).toString();

		ChangeAmountDlg dlg( oldAmount, this );

		if( dlg.exec() == QDialog::Accepted )
		{
			DbResult res = d->m_db->changeProduct( { code, place, dlg.amount(), desc } );

			if( !res.m_ok )
			{
				QMessageBox::critical( this, tr( "Error in the database..." ),
					tr( "Unable to change amount of product.\n\n%1" )
						.arg( res.m_error ) );
			}
			else
				d->m_sigs->emitProductChanged( code, place, dlg.amount(), desc );
		}
	}
}

} /* namespace Stock */
