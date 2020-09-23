
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
#include <QAbstractVideoSurface>
#include <QMutex>
#include <QCamera>


namespace Stock {

//
// Frames
//

//! Frames listener.
class Frames
	:	public QAbstractVideoSurface
{
	Q_OBJECT

	Q_PROPERTY( QAbstractVideoSurface * videoSurface READ videoSurface WRITE setVideoSurface )

signals:
	//! Code detected.
	void codeDetected( const QString & code );

public:
	static void registerQmlType();

	explicit Frames( QObject * parent = nullptr );
	~Frames() override;

	bool present( const QVideoFrame & frame ) override;

	QList< QVideoFrame::PixelFormat > supportedPixelFormats(
		QAbstractVideoBuffer::HandleType type =
			QAbstractVideoBuffer::NoHandle ) const override;

	//! \return Video surface.
	QAbstractVideoSurface * videoSurface() const;
	//! Set video surface.
	void setVideoSurface( QAbstractVideoSurface * s );

public slots:
	//! Emit code.
	void emitCode( const QString & code );

private:
	Q_DISABLE_COPY( Frames )

	//! Mutex.
	QMutex m_mutex;
	//! QML surface.
	QAbstractVideoSurface * m_qml;
	//! Camera.
	QCamera * m_cam;
	//! Counter.
	int m_counter;
}; // class Frames

} /* namespace Stock */

#endif // STOCK_FRAMES_HPP_INCLUDED
