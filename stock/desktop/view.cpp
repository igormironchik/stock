
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
#include "view.hpp"
#include "by_product_sort_model.hpp"
#include "by_product_view.hpp"
#include "by_place_view.hpp"
#include "by_place_model.hpp"
#include "by_place_sort_model.hpp"

// Qt include.
#include <QBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QStackedWidget>
#include <QHeaderView>


namespace Stock {

//
// ViewPrivate
//

class ViewPrivate {
public:
	ViewPrivate( View * parent )
		:	m_stack( Q_NULLPTR )
		,	m_codeView( Q_NULLPTR )
		,	m_placeView( Q_NULLPTR )
		,	m_codeModel( Q_NULLPTR )
		,	m_placeModel( Q_NULLPTR )
		,	m_code( Q_NULLPTR )
		,	m_place( Q_NULLPTR )
		,	m_desc( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	QStackedWidget * m_stack;
	//! By product code tree view.
	ByProductView * m_codeView;
	//! By place tree view.
	ByPlaceView * m_placeView;
	//! By Product filter model.
	ByProductSortModel * m_codeModel;
	//! By Place filter model.
	ByPlaceSortModel * m_placeModel;
	//! Code.
	QLineEdit * m_code;
	//! Place.
	QLineEdit * m_place;
	//! Description.
	QLineEdit * m_desc;
	//! Parent.
	View * q;
}; // class ViewPrivate

void
ViewPrivate::init()
{
	QVBoxLayout * l = new QVBoxLayout( q );

	QHBoxLayout * h = new QHBoxLayout;

	QLabel * filter = new QLabel( q );
	filter->setText( View::tr( "Filter" ) );
	h->addWidget( filter );

	m_code = new QLineEdit( q );
	m_code->setPlaceholderText( View::tr( "Product Code" ) );
	h->addWidget( m_code );

	m_place = new QLineEdit( q );
	m_place->setPlaceholderText( View::tr( "Place" ) );
	h->addWidget( m_place );

	m_desc = new QLineEdit( q );
	m_desc->setPlaceholderText( View::tr( "Description of Product" ) );
	h->addWidget( m_desc );

	l->addLayout( h );

	m_stack = new QStackedWidget( q );

	m_codeView = new ByProductView( m_stack );
	m_codeView->setAllColumnsShowFocus( true );
	m_codeView->setRootIsDecorated( true );
	m_codeView->setAlternatingRowColors( true );
	m_codeView->setSortingEnabled( true );
	m_codeView->setWordWrap( true );
	m_codeView->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_codeView->setSelectionMode( QAbstractItemView::SingleSelection );
	m_codeView->header()->setStretchLastSection( true );
	m_codeView->header()->setSectionResizeMode( QHeaderView::ResizeToContents );
	m_codeView->header()->setSortIndicator( 0, Qt::AscendingOrder );
	m_stack->addWidget( m_codeView );

	m_placeView = new ByPlaceView( m_stack );
	m_placeView->setAllColumnsShowFocus( true );
	m_placeView->setRootIsDecorated( true );
	m_placeView->setAlternatingRowColors( true );
	m_placeView->setSortingEnabled( true );
	m_placeView->setWordWrap( true );
	m_placeView->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_placeView->setSelectionMode( QAbstractItemView::SingleSelection );
	m_placeView->header()->setStretchLastSection( true );
	m_placeView->header()->setSectionResizeMode( QHeaderView::ResizeToContents );
	m_placeView->header()->setSortIndicator( 0, Qt::AscendingOrder );
	m_stack->addWidget( m_placeView );

	l->addWidget( m_stack );

	View::connect( m_code, &QLineEdit::textChanged,
		q, &View::filterChanged );
	View::connect( m_place, &QLineEdit::textChanged,
		q, &View::filterChanged );
	View::connect( m_desc, &QLineEdit::textChanged,
		q, &View::filterChanged );
}


//
// View
//

View::View( QWidget * parent )
	:	QWidget( parent )
	,	d( new ViewPrivate( this ) )
{
	d->init();
}

View::~View()
{
}

ByProductView *
View::byProductsView() const
{
	return d->m_codeView;
}

ByPlaceView *
View::byPlaceView() const
{
	return d->m_placeView;
}

void
View::setFilterModels( ByProductSortModel * product, ByPlaceSortModel * place )
{
	d->m_codeModel = product;
	d->m_placeModel = place;

	d->m_codeView->setFilterModel( d->m_codeModel );
	d->m_placeView->setFilterModel( d->m_placeModel );
}

void
View::setModels( ByProductModel * product,
	ByPlaceModel * place )
{
	d->m_codeView->setSourceModel( product );
	d->m_placeView->setSourceModel( place );
}

void
View::setDb( DbSignals * sigs, Db * db )
{
	d->m_codeView->setDb( sigs, db );
	d->m_placeView->setDb( sigs, db );
}

void
View::filterChanged( const QString & )
{
	if( d->m_codeModel )
		d->m_codeModel->setFilterData( d->m_code->text(),
			d->m_place->text(), d->m_desc->text() );

	if( d->m_placeModel )
		d->m_placeModel->setFilterData( d->m_code->text(),
			d->m_place->text(), d->m_desc->text() );
}

void
View::switchToByProductCode()
{
	d->m_stack->setCurrentIndex( 0 );
}

void
View::switchToByPlace()
{
	d->m_stack->setCurrentIndex( 1 );
}

} /* namespace Stock */
