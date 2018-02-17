
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
#include "product.hpp"
#include "ui_product.h"
#include "by_product_model.hpp"

// Qt include.
#include <QComboBox>
#include <QTextEdit>
#include <QSpinBox>


namespace Stock {

//
// ProductDlgPrivate
//

class ProductDlgPrivate {
public:
	ProductDlgPrivate( ByProductModel * model, ProductDlg * parent )
		:	m_model( model )
		,	q( parent )
	{
	}

	//! Init.
	void init( const QString & code,
		const QString & place,
		quint64 count,
		const QString & desc,
		const QStringList & codes,
		const QStringList & places,
		bool isNewProduct );

	//! Ui.
	Ui::ProductDlg m_ui;
	//! By Product model.
	ByProductModel * m_model;
	//! Parent.
	ProductDlg * q;
}; // class ProductDlgPrivate

void
ProductDlgPrivate::init( const QString & code,
	const QString & place,
	quint64 count,
	const QString & desc,
	const QStringList & codes,
	const QStringList & places,
	bool isNewProduct )
{
	m_ui.setupUi( q );

	m_ui.m_code->addItems( codes );
	m_ui.m_code->setCurrentText( code );
	m_ui.m_place->addItems( places );
	m_ui.m_place->setCurrentText( place );
	m_ui.m_desc->setText( desc );
	m_ui.m_count->setValue( count );

	if( !isNewProduct )
	{
		m_ui.m_code->setEnabled( false );
		m_ui.m_place->setEnabled( false );
	}
}


//
// ProductDlg
//

ProductDlg::ProductDlg( const QString & code,
	const QString & place,
	quint64 count,
	const QString & desc,
	const QStringList & codes,
	const QStringList & places,
	bool isNewProduct,
	ByProductModel * model,
	QWidget * parent )
	:	QDialog( parent )
	,	d( new ProductDlgPrivate( model, this ) )
{
	d->init( code, place, count, desc, codes, places, isNewProduct );

	connect( d->m_ui.m_code, &QComboBox::currentTextChanged,
		this, &ProductDlg::codeChanged );
	connect( d->m_ui.m_place, &QComboBox::currentTextChanged,
		this, &ProductDlg::placeChanged );
}

ProductDlg::~ProductDlg()
{
}

QString
ProductDlg::code() const
{
	return d->m_ui.m_code->currentText();
}

QString
ProductDlg::place() const
{
	return d->m_ui.m_place->currentText();
}

quint64
ProductDlg::count() const
{
	return d->m_ui.m_count->value();
}

QString
ProductDlg::desc() const
{
	return d->m_ui.m_desc->toPlainText();
}

void
ProductDlg::codeChanged( const QString & code )
{
	d->m_ui.m_desc->setText( d->m_model->desc( code ) );

	if( !d->m_ui.m_place->currentText().isEmpty() )
		d->m_ui.m_count->setValue( d->m_model->count( code,
			d->m_ui.m_place->currentText() ) );
}

void
ProductDlg::placeChanged( const QString & place )
{
	if( !d->m_ui.m_code->currentText().isEmpty() )
		d->m_ui.m_count->setValue( d->m_model->count(
			d->m_ui.m_code->currentText(), place ) );
}

} /* namespace Stock */
