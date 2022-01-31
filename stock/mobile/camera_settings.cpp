
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

// Stock include.
#include "camera_settings.hpp"

// Qt include.
#include <QCoreApplication>
#include <QDir>
#include <QRegularExpression>
#include <QMediaDevices>

// cfgfile include.
#include <cfgfile/all.hpp>


namespace Stock {

//
// CameraSettings
//

CameraSettings::CameraSettings()
	:	m_dirty( false )
{
	const auto cameras = QMediaDevices::videoInputs();

	QStringList camerasNames;

	for( const auto & cameraInfo : cameras )
	{
		m_camsInfo.insert( cameraInfo.description(), cameraInfo );
		camerasNames << cameraInfo.description();
	}

	m_camsModel.setStringList( camerasNames );

	for( auto it = m_camsInfo.cbegin(), last = m_camsInfo.cend(); it != last; ++it )
	{
		const auto settings = it.value().videoFormats();

		for( const auto & s : settings )
		{
			if( s.pixelFormat() != QVideoFrameFormat::Format_Jpeg ||
				QVideoFrameFormat::imageFormatFromPixelFormat( s.pixelFormat() ) !=
					QImage::Format_Invalid )
			{
				const auto resolutionStr = resolution( s.resolution().width(),
					s.resolution().height(), s.maxFrameRate() );

				m_resolutions[ it.key() ].insert( resolutionStr, s );
			}
		}
	}

	readCfg();

	updateTransform();
}

CameraSettings::~CameraSettings()
{
}

static CameraSettings * camSettingsPtr = nullptr;

void
CameraSettings::cleanup()
{
	delete camSettingsPtr;

	camSettingsPtr = nullptr;
}

CameraSettings &
CameraSettings::instance()
{
	if( !camSettingsPtr )
	{
		camSettingsPtr = new CameraSettings;

		qAddPostRoutine( &CameraSettings::cleanup );
	}

	return *camSettingsPtr;
}

QStringListModel *
CameraSettings::camsList()
{
	return &m_camsModel;
}

QStringListModel *
CameraSettings::camResolutions()
{
	return &m_camResolutions;
}

CameraSettings::Transform
CameraSettings::transform() const
{
	Transform t { 1.0, 1.0, 0.0 };

	if( m_cfg.mirrored() )
		t.m_xScale = -1.0;

	if( m_cfg.rotation() != 0 )
		t.m_rot = m_cfg.rotation();

	return t;
}

void
CameraSettings::setTransform( int rot, bool mirrored )
{
	m_cfg.set_rotation( rot );
	m_cfg.set_mirrored( mirrored );

	updateTransform();

	emit transformChanged();
}

const QTransform &
CameraSettings::qTransform() const
{
	return m_transform;
}

QCameraFormat
CameraSettings::camSettings() const
{
	if( !m_cfg.device().isEmpty() && m_cfg.width() != 0 )
	{
		const auto it = m_resolutions.find( m_cfg.device() );

		if( it != m_resolutions.cend() )
			return it->value( resolution( m_cfg.width(), m_cfg.height(), m_cfg.frames() ) );
	}

	return QCameraFormat();
}

QString
CameraSettings::camSettingsStr() const
{
	const auto s = camSettings();

	return resolution( s.resolution().width(), s.resolution().height(), s.maxFrameRate() );
}

void
CameraSettings::setCamSettings( const QString & resolution, bool notify )
{
	static const QRegularExpression parser( QStringLiteral( "^(\\d+)x(\\d+)@(\\d+)$" ) );

	const auto match = parser.match( resolution );

	if( match.hasMatch() )
	{
		m_cfg.set_width( match.captured( 1 ).toInt() );
		m_cfg.set_height( match.captured( 2 ).toInt() );
		m_cfg.set_frames( match.captured( 3 ).toInt() );

		m_dirty = true;

		if( notify )
			emit camSettingsChanged();
	}
}

void
CameraSettings::setCamSettings( const QCameraFormat & s, bool notify )
{
	m_cfg.set_width( s.resolution().width() );
	m_cfg.set_height( s.resolution().height() );
	m_cfg.set_frames( qRound( s.maxFrameRate() ) );

	if( notify )
		emit camSettingsChanged();
}

QString
CameraSettings::camName() const
{
	if( !m_cfg.device().isEmpty() )
		return m_cfg.device();
	else if( m_camsModel.rowCount() )
		return m_camsModel.data( m_camsModel.index( 0, 0 ) ).toString();
	else
		return QString();
}

QCameraDevice
CameraSettings::camInfo( const QString & name ) const
{
	if( m_camsInfo.contains( name ) )
		return m_camsInfo[ name ];
	else
		return QCameraDevice();
}

void
CameraSettings::setCamName( const QString & name, bool notify )
{
	if( m_camsInfo.contains( name ) )
	{
		m_cfg.set_device( name );

		QStringList resolutions;

		for( const auto & s : qAsConst( m_resolutions[ name ] ) )
			resolutions.append( resolution( s.resolution().width(),
				s.resolution().height(), s.maxFrameRate() ) );

		m_camResolutions.setStringList( resolutions );

		m_dirty = true;

		if( notify )
			emit camSettingsChanged();
	}
}

QString
CameraSettings::resolution( int width, int height, qreal fps ) const
{
	return QStringLiteral( "%1x%2@%3" )
		.arg( QString::number( width ),
			QString::number( height ),
			QString::number( qRound( fps ) ) );
}

void
CameraSettings::readCfg()
{
	QFile file( cfgFileName() );

	if( file.open( QIODevice::ReadOnly ) )
	{
		QTextStream s( &file );

		try {
			tag_CameraCfg< cfgfile::qstring_trait_t> tag;

			cfgfile::read_cfgfile( tag, s, cfgFileName() );

			m_cfg = tag.get_cfg();

			file.close();
		}
		catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
		{
			file.close();
		}
	}
}

void
CameraSettings::writeCfg()
{
	QFile file( cfgFileName() );

	if( file.open( QIODevice::WriteOnly ) )
	{
		QTextStream s( &file );

		try {
			tag_CameraCfg< cfgfile::qstring_trait_t> tag( m_cfg );

			cfgfile::write_cfgfile( tag, s );

			file.close();
		}
		catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
		{
			file.close();
		}
	}
}

QString
CameraSettings::cfgFileName() const
{
	const auto configsPath = QLatin1String( "./Stock" );

	QDir dir( configsPath );

	if( !dir.exists() )
		dir.mkpath( QLatin1String( "." ) );

	return ( configsPath + QLatin1String( "/camera.cfg" ) );
}

void
CameraSettings::applySettings()
{
	if( m_dirty )
	{
		m_dirty = false;

		writeCfg();

		updateTransform();

		emit camSettingsChanged();
	}
}

void
CameraSettings::updateTransform()
{
	m_transform = QTransform();

	if( m_cfg.mirrored() )
		m_transform.scale( -1.0, 1.0 );
	else
		m_transform.scale( 1.0, 1.0 );

	m_transform.rotate( m_cfg.rotation() );
}

void
CameraSettings::clearDirtyFlag()
{
	m_dirty = false;
}

void
CameraSettings::rotate()
{
	const auto r = m_cfg.rotation() + 90;
	m_cfg.set_rotation( r < 360 ? r : r - 360 );

	updateTransform();

	m_dirty = true;

	emit transformChanged();
}

void
CameraSettings::mirror()
{
	const auto m = !m_cfg.mirrored();
	m_cfg.set_mirrored( m );

	updateTransform();

	m_dirty = true;

	emit transformChanged();
}

} /* namespace Stock */
