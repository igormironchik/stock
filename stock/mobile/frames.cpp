
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
#include <QQmlEngine>
#include <QRunnable>
#include <QMetaObject>
#include <QCameraDevice>
#include <QThreadPool>

// QZXing include.
#include <QZXing.h>


namespace Stock {

static const int c_framesCount = 30;
static const int c_keyFrameMax = 60;

//
// Frames
//

Frames::Frames( QObject * parent )
	:	QVideoSink( parent )
	,	m_cam( nullptr )
	,	m_counter( 0 )
	,	m_keyFrameCounter( 0 )
	,	m_dirty( true )
{
	connect( &CameraSettings::instance(), &CameraSettings::camSettingsChanged,
		this, &Frames::camSettingsChanged );

	connect( this, &QVideoSink::videoFrameChanged,
			this, &Frames::newFrame );

	m_transform = CameraSettings::instance().transform();

	initCam();

	CameraSettings::instance().setCamName( CameraSettings::instance().camName(), false );
	CameraSettings::instance().clearDirtyFlag();
}

Frames::~Frames()
{
	stopCam();
}

QVideoSink *
Frames::videoSink() const
{
	return m_videoSink.get();
}

void
Frames::setVideoSink( QVideoSink * newVideoSink )
{
	if( m_videoSink == newVideoSink )
		return;

	m_videoSink = newVideoSink;

	emit videoSinkChanged();
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
}; // class DetectCode


class DetectChange final
	:	public QRunnable
{
public:
	DetectChange( const QImage & img1, const QImage & img2, Frames * provider )
		:	m_img1( img1 )
		,	m_img2( img2 )
		,	m_provider( provider )
	{
		setAutoDelete( true );
	}

	void run() override
	{
		bool changed = false;

		if( m_img1.width() != m_img2.width() || m_img1.height() != m_img2.height() )
			changed = true;
		else
		{
			qreal diff = 0.0;

			for( int i = 0; i < m_img1.width(); ++i )
			{
				for( int j = 0; j < m_img1.height(); ++j )
					diff += qAbs( (qreal) m_img1.pixel( i, j ) - (qreal) m_img2.pixel( i, j ) );
			}

			diff /= ( (qreal) m_img1.width() * (qreal) m_img2.height() );

			if( diff > 1000000.0 )
				changed = true;
		}

		if( changed )
			QMetaObject::invokeMethod( m_provider, "emitImageChanged", Qt::QueuedConnection );
	}

private:
	//! Image 1.
	QImage m_img1;
	//! Image 2.
	QImage m_img2;
	//! Provider.
	Frames * m_provider;
}; // class DetectChange

} /* namespace anonymous */

void
Frames::newFrame( const QVideoFrame & frame )
{
	qDebug() << "new frame";
	QVideoFrame f = frame;
	f.map( QVideoFrame::ReadOnly );

	const auto fmt = QVideoFrameFormat::imageFormatFromPixelFormat( f.pixelFormat() );

	QImage image;

	if( fmt != QImage::Format_Invalid )
		image = QImage( f.bits( f.planeCount() - 1 ), f.width(), f.height(),
			f.bytesPerLine( f.planeCount() - 1 ), fmt );
//	else if( f.pixelFormat() == QVideoFrameFormat::Format_ARGB8888 )
//	{
//		const auto max = f.width() * f.height() * 4;
//		std::vector< uchar > buf;
//		buf.reserve( max );
//		uchar * bits = f.bits( f.planeCount() - 1 );

//		static const size_t i1 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 2 : 0 );
//		static const size_t i2 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 1 : 3 );
//		static const size_t i3 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 0 : 2 );
//		static const size_t i4 = ( Q_BYTE_ORDER == Q_LITTLE_ENDIAN ? 3 : 1 );

//		for( auto i = 0; i < max; )
//		{
//			buf.push_back( bits[ i1 ] );
//			buf.push_back( bits[ i2 ] );
//			buf.push_back( bits[ i3 ] );
//			buf.push_back( bits[ i4 ] );

//			bits += 4;
//			i += 4;
//		}

//		image = QImage( &buf[ 0 ], f.width(), f.height(), f.bytesPerLine( f.planeCount() - 1 ),
//			QImage::Format_ARGB32 ).copy();
//	}
	else
		image.loadFromData( f.bits( f.planeCount() - 1 ), f.mappedBytes( f.planeCount() - 1 ) );

	f.unmap();

	m_currentFrame = image.copy();

	image = image.transformed( m_transform );

	if( m_counter == 0 )
	{
		auto * detect = new DetectCode( image.copy(), this );
		QThreadPool::globalInstance()->start( detect );
	}

	++m_counter;

	if( m_counter == c_framesCount )
		m_counter = 0;

	if( m_keyFrameCounter == 0 )
		m_keyFrame = m_currentFrame;


	++m_keyFrameCounter;

	if( m_keyFrameCounter == c_keyFrameMax )
	{
		auto * detect = new DetectChange( m_keyFrame.copy(), m_currentFrame.copy(), this );
		QThreadPool::globalInstance()->start( detect );

		m_keyFrameCounter = 0;
	}

	if( m_videoSink )
		m_videoSink->setVideoFrame( frame );
}

QImage
Frames::currentFrame() const
{
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
Frames::emitImageChanged()
{
	emit imageChanged();
}

void
Frames::camSettingsChanged()
{
	const auto i = m_cam->cameraDevice();

	if( CameraSettings::instance().camName() != i.description() )
	{
		stopCam();
		initCam();
	}
	else
	{
		m_cam->stop();
		m_cam->setCameraFormat( CameraSettings::instance().camSettings() );
		m_cam->start();
	}

	m_transform = CameraSettings::instance().transform();

	m_dirty = true;
}

void
Frames::initCam()
{
	const auto camDev = CameraSettings::instance().camName();

	if( !camDev.isEmpty() )
		m_cam = new QCamera( CameraSettings::instance().camInfo( camDev ), this );
	else
		m_cam = new QCamera( this );

	const auto s = m_cam->cameraDevice().videoFormats();

	m_cam->setFocusMode( QCamera::FocusModeAuto );
	//m_cam->setCameraFormat( CameraSettings::instance().camSettings() );
	m_cam->setCameraFormat( s.at( 13 ) );
	m_capture.setCamera( m_cam );
	m_capture.setVideoSink( this );

	m_cam->start();
}

void
Frames::stopCam()
{
	m_cam->stop();

	disconnect( m_cam, 0, 0, 0 );
	m_cam->setParent( nullptr );

	delete m_cam;

	m_cam = nullptr;
}

} /* namespace Stock */
