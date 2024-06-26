
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "by_place_view.hpp"
#include "by_place_model.hpp"
#include "by_place_sort_model.hpp"
#include "by_product_model.hpp"
#include "rename.hpp"
#include "db.hpp"
#include "db_signals.hpp"
#include "edit_desc.hpp"
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
	ByPlaceModel * m_model;
	//! Filter model.
	ByPlaceSortModel * m_filter;
	//! Auxiliary model.
	ByProductModel * m_auxModel;
	//! DB signals.
	DbSignals * m_sigs;
	//! DB.
	Db * m_db;
	//! Delegate for description.
	WordWrapItemDelegate * m_delegate;
	//! Parent.
	ByPlaceView * q;
}; // class ByPlaceViewPrivate

void
ByPlaceViewPrivate::init()
{
	m_delegate = new WordWrapItemDelegate( q );
	q->setItemDelegateForColumn( m_model->columnCount() - 1, m_delegate );
}


//
// ByPlaceView
//

ByPlaceView::ByPlaceView( QWidget * parent )
	:	QTreeView( parent )
	,	d( new ByPlaceViewPrivate( this ) )
{
	d->init();

	connect( header(), &QHeaderView::sectionResized,
		this, &ByPlaceView::sectionResized );
	connect( this, &ByPlaceView::updateDescColumn,
		this, &ByPlaceView::sectionResized );
}

ByPlaceView::~ByPlaceView()
{
}

void
ByPlaceView::sectionResized( int section, int, int )
{
	if( section == d->m_model->columnCount() - 1 )
	{
		for( int i = 0; i < d->m_model->rowCount(); ++i )
			emit d->m_delegate->sizeHintChanged( d->m_model->index( i, section ) );
	}
}

void
ByPlaceView::setFilterModel( ByPlaceSortModel * filter )
{
	d->m_filter = filter;

	setModel( d->m_filter );

	header()->resizeSections( QHeaderView::ResizeToContents );
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
			menu.addAction( QIcon( ":/img/list-add_22x22.png" ),
				tr( "Change Amount" ), this, &ByPlaceView::changeAmount );
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
	if( d->m_db && d->m_sigs && d->m_index.isValid() && d->m_parentIndex.isValid() )
	{
		const auto oldCode = d->m_model->data(
			d->m_model->index( d->m_index.row(), 0, d->m_parentIndex ) ).toString();

		RenameDlg dlg( oldCode, d->m_auxModel->codes(), tr( "Code" ), this );
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
ByPlaceView::changeDesc()
{
	if( d->m_db && d->m_sigs && d->m_index.isValid() && d->m_parentIndex.isValid() )
	{
		const auto oldDesc = d->m_model->data(
			d->m_model->index( d->m_index.row(), 2, d->m_parentIndex ) ).toString();
		const auto code = d->m_model->data(
			d->m_model->index( d->m_index.row(), 0, d->m_parentIndex ) ).toString();

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
			{
				d->m_sigs->emitProductChanged( code, QString(), 0, dlg.text() );

				emit updateDescColumn( d->m_model->columnCount() - 1, 0, 0 );
			}
		}
	}
}

void
ByPlaceView::renamePlace()
{
	QModelIndex index = d->m_parentIndex;

	if( !index.isValid() )
		index = d->m_index;

	if( d->m_db && d->m_sigs && index.isValid() )
	{
		const auto oldPlace = d->m_model->data(
			d->m_model->index( index.row(), 0 ) ).toString();

		RenameDlg dlg( oldPlace, d->m_model->places(), tr( "Place" ), this );
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
ByPlaceView::deletePlace()
{
	if( d->m_db && d->m_sigs && d->m_index.isValid() )
	{
		const auto place = d->m_model->data( d->m_model->index( d->m_index.row(), 0 ) )
			.toString();

		const auto res = QMessageBox::question( this, tr( "Deletion of Place..." ),
			tr( "You are about to completely remove place from the database.\n\n"
				"Are you sure that you want to delete place \"%1\"?" )
					.arg( place ), QMessageBox::Yes | QMessageBox::No,
				QMessageBox::No );

		if( res == QMessageBox::Yes )
		{
			DbResult res = d->m_db->deletePlace( place );

			if( !res.m_ok )
			{
				QMessageBox::critical( this, tr( "Error in the database..." ),
					tr( "Unable to remove place.\n\n%1" )
						.arg( res.m_error ) );
			}
			else
				d->m_sigs->emitPlaceDeleted( place );
		}
	}
}

void
ByPlaceView::changeAmount()
{
	if( d->m_db && d->m_sigs && d->m_index.isValid() && d->m_parentIndex.isValid() )
	{
		const auto code = d->m_model->data(
			d->m_model->index( d->m_index.row(), 0, d->m_parentIndex ) ).toString();
		const auto desc = d->m_model->data(
			d->m_model->index( d->m_index.row(), 2, d->m_parentIndex ) ).toString();
		const auto oldAmount = d->m_model->data(
			d->m_model->index( d->m_index.row(), 1, d->m_parentIndex ) ).toULongLong();
		const auto place = d->m_model->data( d->m_model->index( d->m_index.row(), 0 ) )
			.toString();

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
