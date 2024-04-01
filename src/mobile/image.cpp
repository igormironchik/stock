
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
	m_transform = CameraSettings::instance().qTransform();

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
	m_transform = CameraSettings::instance().qTransform();
	m_dirty = true;

	update();
}

} /* namespace Stock */
