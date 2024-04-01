
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
