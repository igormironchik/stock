
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_EDIT_DESC_HPP_INCLUDED
#define STOCK_EDIT_DESC_HPP_INCLUDED

// Qt include.
#include <QDialog>

// C++ include.
#include <memory>


namespace Stock {

//
// EditDescDlg
//

class EditDescDlgPrivate;

//! EditDescDlg.
class EditDescDlg Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	EditDescDlg( const QString & text, QWidget * parent );
	virtual ~EditDescDlg();

	//! \return Text.
	QString text() const;

private:
	friend class EditDescDlgPrivate;

	Q_DISABLE_COPY( EditDescDlg )

	std::unique_ptr< EditDescDlgPrivate > d;
}; // class EditDescDlg

} /* namespace Stock */

#endif // STOCK_EDIT_DESC_HPP_INCLUDED
