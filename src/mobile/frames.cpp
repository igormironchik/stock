
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "frames.hpp"

// Qt include.
#include <QQmlEngine>
#include <QRunnable>
#include <QMetaObject>
#include <QCameraDevice>
#include <QThreadPool>
#include <QTimer>

// QZXing include.
#include <QZXing.h>

// C++ include.
#include <vector>


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
	,	m_fps( 0 )
	,	m_timer( new QTimer( this ) )
{
	connect( &CameraSettings::instance(), &CameraSettings::camSettingsChanged,
		this, &Frames::camSettingsChanged );

	connect( this, &QVideoSink::videoFrameChanged,
			this, &Frames::newFrame );

	m_timer->setInterval( 1000 );

	connect( m_timer, &QTimer::timeout, this, &Frames::oneSecondTimer );

	m_timer->start();

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

qreal
Frames::angle() const
{
	return m_transform.m_rot;
}

qreal
Frames::xScale() const
{
	return m_transform.m_xScale;
}

qreal
Frames::yScale() const
{
	return m_transform.m_yScale;
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
		{
			QMetaObject::invokeMethod( m_provider, "emitCode", Qt::QueuedConnection,
				Q_ARG( QString, code ) );
		}
	}

private:
	//! Image.
	QImage m_img;
	//! Provider.
	Frames * m_provider;
}; // class DetectCode

} /* namespace anonymous */

void
Frames::newFrame( const QVideoFrame & frame )
{
	QVideoFrame f = frame;
	f.map( QVideoFrame::ReadOnly );

	if( f.isValid() )
	{
		QImage image = f.toImage();

		f.unmap();

		m_currentFrame = image.copy();

		image = image.transformed( CameraSettings::instance().qTransform() );

		if( m_counter == 0 )
		{
			auto * detect = new DetectCode( image.copy(), this );
			QThreadPool::globalInstance()->start( detect );
		}

		++m_counter;

		if( m_counter == c_framesCount )
			m_counter = 0;

		if( m_videoSink )
			m_videoSink->setVideoFrame( frame );

		++m_fps;
	}
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

	emit xScaleChanged();
	emit yScaleChanged();
	emit angleChanged();
}

void
Frames::initCam()
{
	const auto camDev = CameraSettings::instance().camName();

	if( !camDev.isEmpty() )
		m_cam = new QCamera( CameraSettings::instance().camInfo( camDev ), this );
	else
		m_cam = new QCamera( this );

	m_cam->setFocusMode( QCamera::FocusModeAutoNear );
	m_cam->setCameraFormat( CameraSettings::instance().camSettings() );
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

void
Frames::oneSecondTimer()
{
	m_fpsString = QString::number( m_fps );
	m_fps = 0;
	emit fpsStringChanged();
}

QString
Frames::fpsString() const
{
	return m_fpsString;
}

} /* namespace Stock */
