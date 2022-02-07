
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

#pragma once

// Qt include.
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>

// Stock include.
#include "ui_license_dialog.h"


namespace Stock {

//
// LicenseDialog
//

//! Dialog with list of licenses.
class LicenseDialog
	:	public QDialog
{
	Q_OBJECT

public:
	explicit LicenseDialog( QWidget * parent );
	~LicenseDialog() override = default;

	//! Add license.
	void addLicense( const QString & l );

private:
	//! Ui.
	Ui::LicenseDialog m_ui;
	//! Widget.
	QWidget * m_w;
	//! Layout.
	QVBoxLayout * m_l;
}; // class LicenseDIalog

} /* namespace Stock */
