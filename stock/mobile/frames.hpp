
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

#ifndef STOCK_FRAMES_HPP_INCLUDED
#define STOCK_FRAMES_HPP_INCLUDED

// Qt include.
#include <QVideoSink>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QPointer>


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

signals:
	//! Code detected.
	void codeDetected( const QString & code );
	//! Current frame changed.
	void currentFrameChanged();
	//! Image changed.
	void imageChanged();
	//! Vide sink changed.
	void videoSinkChanged();

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

public slots:
	//! Emit code.
	void emitCode( const QString & code );
	//! Emit image changed.
	void emitImageChanged();

private slots:
	//! Camera settings changed.
	void camSettingsChanged();
	//! Init camera.
	void initCam();
	//! Stop camera.
	void stopCam();
	//! Video frame changed.
	void newFrame( const QVideoFrame & frame );

private:
	Q_DISABLE_COPY( Frames )

	//! Camera.
	QCamera * m_cam;
	//! Counter.
	int m_counter;
	//! Key frame counter.
	int m_keyFrameCounter;
	//! Current frame.
	QImage m_currentFrame;
	//! Key frame.
	QImage m_keyFrame;
	//! Dirty?
	bool m_dirty;
	//! Transform.
	QTransform m_transform;
	//! Capture.
	QMediaCaptureSession m_capture;
	//! Video sink of video output.
	QPointer< QVideoSink > m_videoSink;
}; // class Frames

} /* namespace Stock */

#endif // STOCK_FRAMES_HPP_INCLUDED
