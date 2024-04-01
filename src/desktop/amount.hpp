
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_AMOUNT_HPP_INCLUDED
#define STOCK_AMOUNT_HPP_INCLUDED

// Qt include.
#include <QDialog>

// C++ include.
#include <memory>


namespace Stock {

//
// ChangeAmountDlg
//

class ChangeAmountDlgPrivate;

//! ChangeAmountDlg.
class ChangeAmountDlg Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	ChangeAmountDlg( quint64 oldAmount, QWidget * parent );
	virtual ~ChangeAmountDlg();

	//! \return Amount.
	quint64 amount() const;

private:
	Q_DISABLE_COPY( ChangeAmountDlg )

	std::unique_ptr< ChangeAmountDlgPrivate > d;
}; // class ChangeAmountDlg

} /* namespace Stock */

#endif // STOCK_AMOUNT_HPP_INCLUDED
