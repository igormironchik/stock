
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

// Qt include.
#include <QTreeView>
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
		,	q( parent )
	{
	}

	//! Init.
	void init();

	QStackedWidget * m_stack;
	//! By product code tree view.
	QTreeView * m_codeView;
	//! By place tree view.
	QTreeView * m_placeView;
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

	QLineEdit * place = new QLineEdit( q );
	place->setPlaceholderText( View::tr( "Place" ) );
	h->addWidget( place );

	QLineEdit * code = new QLineEdit( q );
	code->setPlaceholderText( View::tr( "Product Code" ) );
	h->addWidget( code );

	QLineEdit * name = new QLineEdit( q );
	name->setPlaceholderText( View::tr( "Name of Product" ) );
	h->addWidget( name );

	l->addLayout( h );

	m_stack = new QStackedWidget( q );

	m_codeView = new QTreeView( m_stack );
	m_codeView->setAllColumnsShowFocus( true );
	m_codeView->setRootIsDecorated( true );
	m_codeView->setAlternatingRowColors( true );
	m_codeView->setSortingEnabled( true );
	m_codeView->setWordWrap( true );
	m_codeView->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_codeView->setSelectionMode( QAbstractItemView::SingleSelection );
	m_codeView->header()->setStretchLastSection( true );
	m_codeView->header()->setSectionResizeMode( QHeaderView::ResizeToContents );
	m_stack->addWidget( m_codeView );

	m_placeView = new QTreeView( m_stack );
	m_placeView->setAllColumnsShowFocus( true );
	m_placeView->setRootIsDecorated( true );
	m_placeView->setAlternatingRowColors( true );
	m_placeView->setSortingEnabled( true );
	m_placeView->setWordWrap( true );
	m_placeView->setSelectionBehavior( QAbstractItemView::SelectRows );
	m_placeView->setSelectionMode( QAbstractItemView::SingleSelection );
	m_placeView->header()->setStretchLastSection( true );
	m_placeView->header()->setSectionResizeMode( QHeaderView::ResizeToContents );
	m_stack->addWidget( m_placeView );

	l->addWidget( m_stack );

	View::connect( code, &QLineEdit::textChanged,
		q, &View::productCodeChanged );
	View::connect( name, &QLineEdit::textChanged,
		q, &View::nameOfProductChanged );
	View::connect( name, &QLineEdit::textChanged,
		q, &View::placeChanged );
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

QTreeView *
View::byProductsView() const
{
	return d->m_codeView;
}

QTreeView *
View::byPlaceView() const
{
	return d->m_placeView;
}

void
View::productCodeChanged( const QString & txt )
{

}

void
View::nameOfProductChanged( const QString & txt )
{

}

void
View::placeChanged( const QString & txt )
{

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
