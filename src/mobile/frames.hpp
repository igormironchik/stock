
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_FRAMES_HPP_INCLUDED
#define STOCK_FRAMES_HPP_INCLUDED

// Qt include.
#include <QVideoSink>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QPointer>

// Stock include.
#include "camera_settings.hpp"


namespace Stock {

//
// Frames
//

//! Frames listener.
class Frames
	:	public QVideoSink
{
	Q_OBJECT

	Q_PROPERTY( QImage currentFrame READ currentFrame NOTIFY currentFrameChanged )
	Q_PROPERTY( QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged )
	Q_PROPERTY( qreal angle READ angle NOTIFY angleChanged )
	Q_PROPERTY( qreal xScale READ xScale NOTIFY xScaleChanged )
	Q_PROPERTY( qreal yScale READ yScale NOTIFY yScaleChanged )
	Q_PROPERTY( QString fpsString READ fpsString NOTIFY fpsStringChanged )

signals:
	//! Code detected.
	void codeDetected( const QString & code );
	//! Current frame changed.
	void currentFrameChanged();
	//! Image changed.
	void imageChanged();
	//! Vide sink changed.
	void videoSinkChanged();

	void angleChanged();
	void xScaleChanged();
	void yScaleChanged();
	void fpsStringChanged();

public:
	static void registerQmlType();

	explicit Frames( QObject * parent = nullptr );
	~Frames() override;

	bool present( const QVideoFrame & frame );

	//! \return Current frame.
	QImage currentFrame() const;

	//! \return Sink of video output.
	QVideoSink * videoSink() const;
	//! Set sink of video output.
	void setVideoSink( QVideoSink * newVideoSink );

	//!	\return Rotation angle.
	qreal angle() const;
	//! \return X scale.
	qreal xScale() const;
	//! \return Y scale.
	qreal yScale() const;

	//! \return FPS string.
	QString fpsString() const;

public slots:
	//! Emit code.
	void emitCode( const QString & code );

private slots:
	//! Camera settings changed.
	void camSettingsChanged();
	//! Init camera.
	void initCam();
	//! Stop camera.
	void stopCam();
	//! Video frame changed.
	void newFrame( const QVideoFrame & frame );
	//! 1s timer.
	void oneSecondTimer();

private:
	Q_DISABLE_COPY( Frames )

	//! Camera.
	QCamera * m_cam;
	//! Counter.
	int m_counter;
	//! FPS.
	int m_fps;
	//! FPS string.
	QString m_fpsString;
	//! Current frame.
	QImage m_currentFrame;
	//! Transform.
	CameraSettings::Transform m_transform;
	//! Capture.
	QMediaCaptureSession m_capture;
	//! Video sink of video output.
	QPointer< QVideoSink > m_videoSink;
	//! 1 second timer.
	QTimer * m_timer;
}; // class Frames

} /* namespace Stock */

#endif // STOCK_FRAMES_HPP_INCLUDED
