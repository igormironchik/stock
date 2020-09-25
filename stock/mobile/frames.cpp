
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
#include "camera_settings.hpp"

// Qt include.
#include <QVideoSurfaceFormat>
#include <QQmlEngine>
#include <QRunnable>
#include <QMetaObject>
#include <QCameraFocus>

// QZXing include.
#include <QZXing.h>


namespace Stock {

static const int c_framesCount = 30;

//
// Frames
//

Frames::Frames( QObject * parent )
	:	QAbstractVideoSurface( parent )
	,	m_qml( nullptr )
	,	m_cam( nullptr )
	,	m_counter( 0 )
	,	m_dirty( true )
{
	connect( &CameraSettings::instance(), &CameraSettings::camSettingsChanged,
		this, &Frames::camSettingsChanged );

	initCam();

	CameraSettings::instance().setCamName( CameraSettings::instance().camName(), false );
}

Frames::~Frames()
{
	stopCam();

	if( m_qml )
		m_qml->stop();
}

namespace /* anonymous */ {

class DetectCode final
	:	public QRunnable
{
public:
	DetectCode( const QImage & img, Frames * provider )
		:	m_img( img )
		,	m_provider( provider )
	{
		setAutoDelete( true );
	}

	void run() override
	{
		QZXing decoder;
		decoder.setDecoder( QZXing::DecoderFormat_Aztec |
			QZXing::DecoderFormat_CODABAR |
			QZXing::DecoderFormat_CODE_39 |
			QZXing::DecoderFormat_CODE_93 |
			QZXing::DecoderFormat_CODE_128 |
			QZXing::DecoderFormat_DATA_MATRIX |
			QZXing::DecoderFormat_EAN_8 |
			QZXing::DecoderFormat_EAN_13 |
			QZXing::DecoderFormat_ITF |
			QZXing::DecoderFormat_MAXICODE |
			QZXing::DecoderFormat_PDF_417 |
			QZXing::DecoderFormat_QR_CODE |
			QZXing::DecoderFormat_RSS_14 |
			QZXing::DecoderFormat_RSS_EXPANDED |
			QZXing::DecoderFormat_UPC_A |
			QZXing::DecoderFormat_UPC_E |
			QZXing::DecoderFormat_UPC_EAN_EXTENSION );

		decoder.setTryHarder( true );

		const auto code = decoder.decodeImage( m_img );

		if( !code.isEmpty() )
			QMetaObject::invokeMethod( m_provider, "emitCode", Qt::QueuedConnection,
				Q_ARG( QString, code ) );
	}

private:
	//! Image.
	QImage m_img;
	//! Provider.
	Frames * m_provider;
}; // class ReadGIF

} /* namespace anonymous */

bool
Frames::present( const QVideoFrame & frame )
{
	if( !isActive() )
		return false;

	QVideoFrame f = frame;
	f.map( QAbstractVideoBuffer::ReadOnly );

	const auto fmt = QVideoFrame::imageFormatFromPixelFormat( f.pixelFormat() );

	QImage image;

	if( fmt != QImage::Format_Invalid )
		image = QImage( f.bits(), f.width(), f.height(), f.bytesPerLine(), fmt );
	else if( f.pixelFormat() == QVideoFrame::Format_ABGR32 )
	{
		const auto max = f.width() * f.height() * 4;
		std::vector< uchar > buf;
		buf.reserve( max );
		uchar * bits = f.bits();

		static const size_t i1 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 2 : 0 );
		static const size_t i2 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 1 : 3 );
		static const size_t i3 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 0 : 2 );
		static const size_t i4 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 3 : 1 );

		for( auto i = 0; i < max; )
		{
			buf.push_back( bits[ i1 ] );
			buf.push_back( bits[ i2 ] );
			buf.push_back( bits[ i3 ] );
			buf.push_back( bits[ i4 ] );

			bits += 4;
			i += 4;
		}

		image = QImage( &buf[ 0 ], f.width(), f.height(), f.bytesPerLine(),
			QImage::Format_ARGB32 ).copy();
	}
	else
	{
		f.unmap();

		return true;
	}

	f.unmap();

	if( m_counter == 0 )
	{
		auto * detect = new DetectCode( image.copy(), this );
		QThreadPool::globalInstance()->start( detect );
	}

	++m_counter;

	if( m_counter == c_framesCount )
		m_counter = 0;

	QMutexLocker lock( &m_mutex );

	if( m_qml )
	{
		if( m_dirty )
		{
			QVideoFrame::PixelFormat pixelFormat =
				QVideoFrame::pixelFormatFromImageFormat( image.format() );

			const auto fmt = QVideoSurfaceFormat( image.size(),
				pixelFormat );

			m_qml->start( fmt );

			m_dirty = false;
		}

		m_qml->present( QVideoFrame( image.copy() ) );
	}

	m_currentFrame = image.copy();

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

QImage
Frames::currentFrame() const
{
	QMutexLocker lock( &m_mutex );

	return m_currentFrame;
}

void
Frames::registerQmlType()
{
	qmlRegisterType< Stock::Frames > ( "Frames", 0, 1, "Frames" );
}

void
Frames::emitCode( const QString & code )
{
	emit codeDetected( code );
}

void
Frames::camStatusChanged( QCamera::Status st )
{
	if( st == QCamera::LoadedStatus )
	{
		const auto s = m_cam->viewfinderSettings();

		CameraSettings::instance().setCamSettings( s, false );
	}
}

void
Frames::camSettingsChanged()
{
	const auto i = QCameraInfo( *m_cam );

	if( CameraSettings::instance().camName() != i.deviceName() )
	{
		stopCam();
		initCam();
	}
	else
		m_cam->setViewfinderSettings( CameraSettings::instance().camSettings() );
}

void
Frames::initCam()
{
	const auto camDev = CameraSettings::instance().camName();

	if( !camDev.isEmpty() )
		m_cam = new QCamera( CameraSettings::instance().camInfo( camDev ), this );
	else
		m_cam = new QCamera( this );

	connect( m_cam, &QCamera::statusChanged, this, &Frames::camStatusChanged );

	m_cam->setCaptureMode( QCamera::CaptureViewfinder );
	m_cam->focus()->setFocusMode( QCameraFocus::AutoFocus );
	m_cam->setViewfinderSettings( CameraSettings::instance().camSettings() );
	m_cam->setViewfinder( this );
	m_cam->start();
}

void
Frames::stopCam()
{
	m_cam->stop();
	m_cam->unload();

	disconnect( m_cam, 0, 0, 0 );
	m_cam->setParent( nullptr );

	delete m_cam;
}

} /* namespace Stock */
