
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
#include "options.hpp"
#include "ui_options.h"

// Qt include.
#include <QLineEdit>
#include <QSpinBox>
#include <QToolButton>


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
	void init( const QString & host, quint16 port, const QString & pwd );

	//! Ui.
	Ui::Options m_ui;
	//! Parent.
	Options * q;
}; // class OptionsPrivate

void
OptionsPrivate::init( const QString & host, quint16 port, const QString & pwd )
{
	m_ui.setupUi( q );

	m_ui.m_host->setText( host );
	m_ui.m_port->setValue( port );
	m_ui.m_password->setText( pwd );

	QObject::connect( m_ui.m_showHidePwd, &QToolButton::clicked,
		q, &Options::showHidePassword );
}


//
// Options
//

Options::Options( const QString & host, quint16 port, const QString & pwd, QWidget * parent )
	:	QDialog( parent )
	,	d( new OptionsPrivate( this ) )
{
	d->init( host, port, pwd );
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

QString
Options::password() const
{
	return d->m_ui.m_password->text();
}

void
Options::showHidePassword()
{
	if( d->m_ui.m_password->echoMode() == QLineEdit::Password )
	{
		d->m_ui.m_password->setEchoMode( QLineEdit::Normal );
		d->m_ui.m_showHidePwd->setIcon( QIcon( ":/img/layer-visible-off_16x16.png" ) );
	}
	else
	{
		d->m_ui.m_password->setEchoMode( QLineEdit::Password );
		d->m_ui.m_showHidePwd->setIcon( QIcon( ":/img/layer-visible-on_16x16.png" ) );
	}
}

} /* namespace Stock */
