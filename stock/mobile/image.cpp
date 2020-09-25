
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

// SecurityCam include.
#include "image.hpp"
#include "camera_settings.hpp"

// Qt include.
#include <QQmlEngine>
#include <QPainter>


namespace Stock {

//
// PreviewImage
//

PreviewImage::PreviewImage( QQuickItem * parent )
	:	QQuickPaintedItem( parent )
	,	m_dirty( true )
{
	m_transform = CameraSettings::instance().transform();

	connect( this, &PreviewImage::widthChanged,
		this, &PreviewImage::markDirty );
	connect( this, &PreviewImage::heightChanged,
		this, &PreviewImage::markDirty );
	connect( &CameraSettings::instance(), &CameraSettings::transformChanged,
		this, &PreviewImage::transformChanged );
}

PreviewImage::~PreviewImage()
{
}

QImage
PreviewImage::image() const
{
	return m_image;
}

void
PreviewImage::setImage( const QImage & img )
{
	m_image = img;
	m_dirty = true;

	update();
}

void
PreviewImage::paint( QPainter * painter )
{
	if( m_dirty )
	{
		m_thumbnail = m_image.transformed( m_transform ).scaled( width(), height(),
			Qt::KeepAspectRatio, Qt::SmoothTransformation );
		m_dirty = false;
	}

	painter->drawImage( QRect(
		( m_thumbnail.width() < width() ?
			qRound( (qreal)( width() - m_thumbnail.width() ) / 2.0 ) : 0 ),
		( m_thumbnail.height() < height() ?
			qRound( (qreal)( height() - m_thumbnail.height() ) / 2.0 ) : 0 ),
		m_thumbnail.width(), m_thumbnail.height() ), m_thumbnail );
}

void
PreviewImage::markDirty()
{
	m_dirty = true;

	update();
}

void
PreviewImage::registerQmlType()
{
	qmlRegisterType< Stock::PreviewImage > ( "PreviewImage", 0, 1, "PreviewImage" );
}

void
PreviewImage::transformChanged()
{
	m_transform = CameraSettings::instance().transform();
	m_dirty = true;

	update();
}

} /* namespace Stock */
