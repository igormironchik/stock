
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
#include "rename.hpp"
#include "ui_rename.h"

// Qt include.
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QPalette>


namespace Stock {

//
// RenameDlgPrivate
//

class RenameDlgPrivate {
public:
	RenameDlgPrivate( const QStringList & constraint, RenameDlg * parent )
		:	m_constraint( constraint )
		,	q( parent )
	{
	}

	//! Init.
	void init( const QString & oldName );

	//! Constraint.
	QStringList m_constraint;
	//! Ui.
	Ui::RenameDlg m_ui;
	//! Normal color.
	QColor m_textColor;
	//! Parent.
	RenameDlg * q;
}; // class RenameDlgPrivate

void
RenameDlgPrivate::init( const QString & oldName )
{
	m_ui.setupUi( q );

	m_ui.m_old->setText( oldName );

	m_textColor = m_ui.m_new->palette().color( QPalette::Text );

	QObject::connect( m_ui.m_new, &QLineEdit::textChanged,
		q, &RenameDlg::nameChanged );
}


//
// RenameDlg
//

RenameDlg::RenameDlg( const QString & oldName, const QStringList & constraint,
	QWidget * parent )
	:	QDialog( parent )
	,	d( new RenameDlgPrivate( constraint, this ) )
{
	d->init( oldName );
}

RenameDlg::~RenameDlg()
{
}

QString
RenameDlg::renamed() const
{
	return d->m_ui.m_new->text();
}

void
RenameDlg::nameChanged( const QString & txt )
{
	if( d->m_constraint.contains( txt ) )
	{
		QPalette palette = d->m_ui.m_new->palette();
		palette.setColor( QPalette::Text, Qt::red );
		d->m_ui.m_new->setPalette( palette );

		d->m_ui.m_btns->button( QDialogButtonBox::Ok )->setEnabled( false );
	}
	else
	{
		QPalette palette = d->m_ui.m_new->palette();
		palette.setColor( QPalette::Text, d->m_textColor );
		d->m_ui.m_new->setPalette( palette );

		d->m_ui.m_btns->button( QDialogButtonBox::Ok )->setEnabled( true );
	}
}

} /* namespace Stock */
