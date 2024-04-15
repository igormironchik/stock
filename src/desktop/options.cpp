
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "options.hpp"
#include "ui_options.h"

// Qt include.
#include <QLineEdit>
#include <QSpinBox>
#include <QToolButton>
#include <QDialogButtonBox>
#include <QPushButton>


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
	
	q->hostPasswordChanged( {} );

	QObject::connect( m_ui.m_showHidePwd, &QToolButton::clicked,
		q, &Options::showHidePassword );
	QObject::connect( m_ui.m_host, &QLineEdit::textChanged,
		q, &Options::hostPasswordChanged );
	QObject::connect( m_ui.m_password, &QLineEdit::textChanged,
		q, &Options::hostPasswordChanged );
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

void
Options::hostPasswordChanged( const QString & )
{
	const auto isOk = !d->m_ui.m_host->text().isEmpty() && !d->m_ui.m_password->text().isEmpty();
	
	d->m_ui.m_buttonBox->button( QDialogButtonBox::Ok )->setEnabled( isOk );	
}

} /* namespace Stock */
