
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018-2020 Igor Mironchik

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
