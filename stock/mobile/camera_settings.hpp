
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

#ifndef STOCK_CAMERA_SETTINGS_HPP_INCLUDED
#define STOCK_CAMERA_SETTINGS_HPP_INCLUDED

// Qt include.
#include <QObject>
#include <QStringListModel>
#include <QMap>
#include <QCameraDevice>

// Stock include.
#include "camera.hpp"


namespace Stock {

//
// CameraSettings
//

//! Camera settings.
class CameraSettings final
	:	public QObject
{
	Q_OBJECT

	Q_PROPERTY( QString currentCamName READ camName WRITE setCamName NOTIFY camSettingsChanged )
	Q_PROPERTY( QString currentResolution READ camSettingsStr WRITE setCamSettings NOTIFY camSettingsChanged )

	CameraSettings();
	~CameraSettings();

	static void cleanup();

signals:
	//! Transform changed.
	void transformChanged();
	//! Camera settings changed.
	void camSettingsChanged();

public:
	static CameraSettings & instance();

	//! \return List of cameras.
	QStringListModel * camsList();
	//! \return List of resolutions.
	QStringListModel * camResolutions();

	//! \return Transform.
	QTransform transform() const;
	//! Set transform.
	void setTransform( int rot, bool mirrored );

	//! \return Camera settings.
	QCameraFormat camSettings() const;
	//! \return Camera settings string.
	QString camSettingsStr() const;
	//! Set camera settings.
	void setCamSettings( const QCameraFormat & s, bool notify = true );

	//! \return Camera name.
	QString camName() const;

	//! \return Camera info.
	QCameraDevice camInfo( const QString & name ) const;

	//! \return Resolution string.
	QString resolution( int width, int height, qreal fps ) const;

public slots:
	//! Set camera name.
	void setCamName( const QString & name, bool notify = true );
	//! Set camera settings.
	void setCamSettings( const QString & resolution, bool notify = true );
	//! Apply settings if changed.
	void applySettings();
	//! Clear dirty flag.
	void clearDirtyFlag();
	//! Rotate.
	void rotate();
	//! Mirror.
	void mirror();

private:
	//! Check camera formats.
	void checkCameraFormats();
	//! Read configuration.
	void readCfg();
	//! Write configuration.
	void writeCfg();
	//! \return Configuration file name.
	QString cfgFileName() const;

private:
	Q_DISABLE_COPY( CameraSettings )

	//! List of cameras.
	QStringListModel m_camsModel;
	//! List of resolutions.
	QStringListModel m_camResolutions;
	//! Cameras.
	QMap< QString, QCameraDevice > m_camsInfo;
	//! Cameras iterator.
	QMap< QString, QCameraDevice >::ConstIterator m_camsInfoIt;
	//! Resolutions.
	QMap< QString, QMap< QString, QCameraFormat > > m_resolutions;
	//! Configuration.
	CameraCfg m_cfg;
	//! Dirty?
	bool m_dirty;
}; // class CameraSettings

} /* namespace Stock */

#endif // STOCK_CAMERA_SETTINGS_HPP_INCLUDED
