
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
	void init( const QString & oldName, const QString & varName );

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
RenameDlgPrivate::init( const QString & oldName, const QString & varName )
{
	m_ui.setupUi( q );

	m_ui.m_old->setText( oldName );
	m_ui.m_oldLabel->setText( RenameDlg::tr( "Old %1" ).arg( varName ) );
	m_ui.m_newLabel->setText( RenameDlg::tr( "New %1" ).arg( varName ) );
	m_ui.m_btns->button( QDialogButtonBox::Ok )->setEnabled( false );
	m_ui.m_new->setFocus();

	m_textColor = m_ui.m_new->palette().color( QPalette::Text );

	QObject::connect( m_ui.m_new, &QLineEdit::textChanged,
		q, &RenameDlg::nameChanged );
}


//
// RenameDlg
//

RenameDlg::RenameDlg( const QString & oldName, const QStringList & constraint,
	const QString & varName, QWidget * parent )
	:	QDialog( parent )
	,	d( new RenameDlgPrivate( constraint, this ) )
{
	d->init( oldName, varName );
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
	if( d->m_constraint.contains( txt ) || txt.isEmpty() )
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
