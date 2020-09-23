
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
#include "frames.hpp"

// Qt include.
#include <QVideoSurfaceFormat>
#include <QQmlEngine>

#include <QDebug>


namespace Stock {

//
// Frames
//

Frames::Frames( QObject * parent )
	:	QAbstractVideoSurface( parent )
	,	m_qml( nullptr )
	,	m_cam( new QCamera( this ) )
{
	m_cam->setViewfinder( this );
	m_cam->start();
}

Frames::~Frames()
{
	m_cam->stop();

	if( m_qml )
		m_qml->stop();
}

bool
Frames::present( const QVideoFrame & frame )
{
	if( !isActive() )
		return false;

	QVideoFrame f = frame;
	f.map( QAbstractVideoBuffer::ReadOnly );

	QImage image( f.bits(), f.width(), f.height(), f.bytesPerLine(),
		QVideoFrame::imageFormatFromPixelFormat( f.pixelFormat() ) );

	f.unmap();

	QMutexLocker lock( &m_mutex );

	if( m_qml )
	{
		if( !m_qml->isActive() )
		{
			QVideoFrame::PixelFormat pixelFormat =
				QVideoFrame::pixelFormatFromImageFormat( image.format() );

			const auto fmt = QVideoSurfaceFormat( image.size(),
				pixelFormat );

			m_qml->start( fmt );
		}

		m_qml->present( QVideoFrame( image.copy() ) );
	}

	return true;
}

QList< QVideoFrame::PixelFormat >
Frames::supportedPixelFormats( QAbstractVideoBuffer::HandleType type ) const
{
	Q_UNUSED( type )

	return QList< QVideoFrame::PixelFormat > ()
		<< QVideoFrame::Format_ARGB32
		<< QVideoFrame::Format_ARGB32_Premultiplied
		<< QVideoFrame::Format_RGB32
		<< QVideoFrame::Format_RGB24;
}

QAbstractVideoSurface *
Frames::videoSurface() const
{
	return m_qml;
}

void
Frames::setVideoSurface( QAbstractVideoSurface * s )
{
	QMutexLocker lock( &m_mutex );

	if( m_qml && m_qml->isActive() )
		m_qml->stop();

	m_qml = s;
}

void
Frames::registerQmlType()
{
	qmlRegisterType< Stock::Frames > ( "Frames", 0, 1, "Frames" );
}

} /* namespace Stock */
