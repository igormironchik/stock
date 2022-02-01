
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

#ifndef STOCK_RENAME_HPP_INCLUDED
#define STOCK_RENAME_HPP_INCLUDED

// Qt include.
#include <QDialog>

// C++ include.
#include <memory>


namespace Stock {

//
// RenameDlg
//

class RenameDlgPrivate;

//! RenameDlg.
class RenameDlg Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	RenameDlg( const QString & oldName, const QStringList & constraint,
		QWidget * parent );
	virtual ~RenameDlg();

	//! \return New name.
	QString renamed() const;

private slots:
	//! Name changed.
	void nameChanged( const QString & txt );

private:
	friend class RenameDlgPrivate;

	Q_DISABLE_COPY( RenameDlg )

	std::unique_ptr< RenameDlgPrivate > d;
}; // class RenameDlg

} /* namespace Stock */

#endif // STOCK_RENAME_HPP_INCLUDED
