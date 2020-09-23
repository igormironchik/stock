
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

#ifndef STOCK_OPTIONS_HPP_INCLUDED
#define STOCK_OPTIONS_HPP_INCLUDED

// Qt include.
#include <QDialog>
#include <QScopedPointer>


namespace Stock {

//
// Options
//

class OptionsPrivate;

//! Options.
class Options Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	Options( const QString & host, quint16 port, const QString & pwd, QWidget * parent );
	virtual ~Options();

	//! \return Host.
	QString host() const;
	//! \return Port.
	quint16 port() const;
	//! \return Password.
	QString password() const;

private slots:
	//! Show/hide password.
	void showHidePassword();

private:
	friend class OptionsPrivate;

	Q_DISABLE_COPY( Options )

	QScopedPointer< OptionsPrivate > d;
}; // class Options

} /* namespace Stock */

#endif // STOCK_OPTIONS_HPP_INCLUDED
