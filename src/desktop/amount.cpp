
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
#include "amount.hpp"
#include "ui_amount.h"

// Qt include.
#include <QSpinBox>


namespace Stock {

//
// ChangeAmountDlgPrivate
//

class ChangeAmountDlgPrivate {
public:
	ChangeAmountDlgPrivate( ChangeAmountDlg * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init( quint64 oldAmount );

	//! Ui.
	Ui::ChangeAmount m_ui;
	//! Parent.
	ChangeAmountDlg * q;
}; // class ChangeAmountDlgPrivate

void
ChangeAmountDlgPrivate::init( quint64 oldAmount )
{
	m_ui.setupUi( q );

	m_ui.m_count->setValue( oldAmount );
}


//
// ChangeAmountDlg
//

ChangeAmountDlg::ChangeAmountDlg( quint64 oldAmount, QWidget * parent )
	:	QDialog( parent )
	,	d( new ChangeAmountDlgPrivate( this ) )
{
	d->init( oldAmount );
}

ChangeAmountDlg::~ChangeAmountDlg()
{
}

quint64
ChangeAmountDlg::amount() const
{
	return d->m_ui.m_count->value();
}

} /* namespace Stock */
