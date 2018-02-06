
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

// Stock include.
#include "options.hpp"
#include "ui_options.h"

// Qt include.
#include <QLineEdit>
#include <QSpinBox>


namespace Stock {

//
// OptionsPrivate
//

class OptionsPrivate {
public:
	OptionsPrivate( Options * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init( const QString & host, quint16 port );

	//! Ui.
	Ui::Options m_ui;
	//! Parent.
	Options * q;
}; // class OptionsPrivate

void
OptionsPrivate::init( const QString & host, quint16 port )
{
	m_ui.setupUi( q );

	m_ui.m_host->setText( host );
	m_ui.m_port->setValue( port );
}


//
// Options
//

Options::Options( const QString & host, quint16 port, QWidget * parent )
	:	QDialog( parent )
	,	d( new OptionsPrivate( this ) )
{
	d->init( host, port );
}

Options::~Options()
{
}

QString
Options::host() const
{
	return d->m_ui.m_host->text();
}

quint16
Options::port() const
{
	return d->m_ui.m_port->value();
}

} /* namespace Stock */
