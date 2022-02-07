
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
#include "license_dialog.hpp"

// Qt include.
#include <QScrollArea>
#include <QLabel>
#include <QFrame>


namespace Stock {

//
// LicenseDialog
//

LicenseDialog::LicenseDialog( QWidget * parent )
	:	QDialog( parent )
	,	m_w( new QWidget( this ) )
	,	m_l( new QVBoxLayout( m_w ) )
{
	m_ui.setupUi( this );

	m_ui.m_scrollArea->setWidget( m_w );
}

void
LicenseDialog::addLicense( const QString & l )
{
	const bool first = ( m_l->count() == 0 );

	if( !first )
	{
		QFrame * line = new QFrame( m_w );
		line->setFrameStyle( QFrame::HLine | QFrame::Plain );
		m_l->addWidget( line );
	}

	QLabel * label = new QLabel( m_w );
	label->setText( l );
	label->setWordWrap( true );
	label->setTextInteractionFlags( Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard |
		Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard );
	label->setOpenExternalLinks( true );

	m_l->addWidget( label );
}

} /* namespace Stock */
