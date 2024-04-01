
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "edit_desc.hpp"
#include "ui_edit_desc.h"

// Qt include.
#include <QTextEdit>


namespace Stock {

//
// EditDescDlgPrivate
//

class EditDescDlgPrivate {
public:
	EditDescDlgPrivate( EditDescDlg * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init( const QString & text );

	//! Ui.
	Ui::EditDescDlg m_ui;
	//! Parent.
	EditDescDlg * q;
}; // class EditDescDlgPrivate

void
EditDescDlgPrivate::init( const QString & text )
{
	m_ui.setupUi( q );

	m_ui.m_text->setPlainText( text );
}


//
// EditDescDlg
//

EditDescDlg::EditDescDlg( const QString & text, QWidget * parent )
	:	QDialog( parent )
	,	d( new EditDescDlgPrivate( this ) )
{
	d->init( text );
}

EditDescDlg::~EditDescDlg()
{
}

QString
EditDescDlg::text() const
{
	return d->m_ui.m_text->toPlainText();
}

} /* namespace Stock */
