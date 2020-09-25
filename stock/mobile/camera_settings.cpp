
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
#include <QTextCodec>
#include <QRegExp>

// cfgfile include.
#include <cfgfile/all.hpp>


namespace Stock {

//
// CameraSettings
//

CameraSettings::CameraSettings()
	:	m_cam( nullptr )
	,	m_dirty( false )
{
	const auto cameras = QCameraInfo::availableCameras();

	QStringList camerasNames;

	for( const auto & cameraInfo : cameras )
	{
		m_camsInfo.insert( cameraInfo.deviceName(), cameraInfo );
		camerasNames << cameraInfo.deviceName();
	}

	m_camsModel.setStringList( camerasNames );

	m_camsInfoIt = m_camsInfo.cbegin();

	readCfg();

	checkNextCamera();
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

QTransform
CameraSettings::transform() const
{
	QTransform t;

	if( m_cfg.mirrored() )
		t.scale( -1.0, 1.0 );

	if( m_cfg.rotation() != 0 )
		t.rotate( m_cfg.rotation() );

	return t;
}

void
CameraSettings::setTransform( int rot, bool mirrored )
{
	m_cfg.set_rotation( rot );
	m_cfg.set_mirrored( mirrored );

	emit transformChanged();
}

QCameraViewfinderSettings
CameraSettings::camSettings() const
{
	if( !m_cfg.device().isEmpty() && m_cfg.width() != 0 )
	{
		return m_resolutions[ m_cfg.device() ]
			[ resolution( m_cfg.width(), m_cfg.height(), m_cfg.frames() ) ];
	}

	return QCameraViewfinderSettings();
}

QString
CameraSettings::camSettingsStr() const
{
	const auto s = camSettings();

	return resolution( s.resolution().width(), s.resolution().height(), s.maximumFrameRate() );
}

void
CameraSettings::setCamSettings( const QString & resolution, bool notify )
{
	static const QRegExp parser( QStringLiteral( "^(\\d+)x(\\d+)@(\\d+)$" ) );

	if( parser.exactMatch( resolution ) )
	{
		parser.indexIn( resolution );

		m_cfg.set_width( parser.cap( 1 ).toInt() );
		m_cfg.set_height( parser.cap( 2 ).toInt() );
		m_cfg.set_frames( parser.cap( 3 ).toInt() );

		m_dirty = true;

		if( notify )
			emit camSettingsChanged();
	}
}

void
CameraSettings::setCamSettings( const QCameraViewfinderSettings & s, bool notify )
{
	m_cfg.set_width( s.resolution().width() );
	m_cfg.set_height( s.resolution().height() );
	m_cfg.set_frames( qRound( s.maximumFrameRate() ) );

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

QCameraInfo
CameraSettings::camInfo( const QString & name ) const
{
	if( m_camsInfo.contains( name ) )
		return m_camsInfo[ name ];
	else
		return QCameraInfo();
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
				s.resolution().height(), s.maximumFrameRate() ) );

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
CameraSettings::camStatusChanged( QCamera::Status st )
{
	if( st == QCamera::LoadedStatus )
	{
		const auto settings = m_cam->supportedViewfinderSettings();

		for( const auto & s : settings )
		{
			const auto resolutionStr = resolution( s.resolution().width(),
				s.resolution().height(), s.maximumFrameRate() );

			m_resolutions[ m_camsInfoIt.key() ].insert( resolutionStr, s );
		}

		disconnect( m_cam, 0, 0, 0 );
		m_cam->stop();
		m_cam->deleteLater();

		++m_camsInfoIt;

		checkNextCamera();
	}
}

void
CameraSettings::checkNextCamera()
{
	if( m_camsInfoIt != m_camsInfo.cend() )
	{
		m_cam = new QCamera( m_camsInfoIt.value(), this );

		connect( m_cam, &QCamera::statusChanged,
			this, &CameraSettings::camStatusChanged );

		m_cam->start();
	}
}

void
CameraSettings::readCfg()
{
	QFile file( cfgFileName() );

	if( file.open( QIODevice::ReadOnly ) )
	{
		QTextStream s( &file );
		s.setCodec( QTextCodec::codecForName( "UTF-8" ) );

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
		s.setCodec( QTextCodec::codecForName( "UTF-8" ) );

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

		emit camSettingsChanged();
	}
}

void
CameraSettings::clearDirtyFlag()
{
	m_dirty = false;
}

} /* namespace Stock */
