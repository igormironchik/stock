
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

#ifndef STOCK_AMOUNT_HPP_INCLUDED
#define STOCK_AMOUNT_HPP_INCLUDED

// Qt include.
#include <QDialog>
#include <QScopedPointer>


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

	QScopedPointer< ChangeAmountDlgPrivate > d;
}; // class ChangeAmountDlg

} /* namespace Stock */

#endif // STOCK_AMOUNT_HPP_INCLUDED
