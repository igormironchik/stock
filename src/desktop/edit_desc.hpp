
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
