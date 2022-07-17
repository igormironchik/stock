
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
// pixelFormatToString
//

QString
pixelFormatToString( QVideoFrameFormat::PixelFormat f )
{
	switch( f )
	{
		case QVideoFrameFormat::Format_ARGB8888 :
			return QStringLiteral( "ARGB8888" );

		case QVideoFrameFormat::Format_ARGB8888_Premultiplied :
			return QStringLiteral( "ARGB8888P" );

		case QVideoFrameFormat::Format_XRGB8888 :
			return QStringLiteral( "XRGB8888" );

		case QVideoFrameFormat::Format_BGRA8888 :
			return QStringLiteral( "BGRA8888" );

		case QVideoFrameFormat::Format_BGRA8888_Premultiplied :
			return QStringLiteral( "BGRA8888P" );

		case QVideoFrameFormat::Format_BGRX8888 :
			return QStringLiteral( "BGRX8888" );

		case QVideoFrameFormat::Format_ABGR8888 :
			return QStringLiteral( "ARGB8888" );

		case QVideoFrameFormat::Format_XBGR8888 :
			return QStringLiteral( "XBGR8888" );

		case QVideoFrameFormat::Format_RGBA8888 :
			return QStringLiteral( "RGBA8888" );

		case QVideoFrameFormat::Format_RGBX8888 :
			return QStringLiteral( "RGBX8888" );

		case QVideoFrameFormat::Format_AYUV :
			return QStringLiteral( "AYUV" );

		case QVideoFrameFormat::Format_AYUV_Premultiplied :
			return QStringLiteral( "AYUVP" );

		case QVideoFrameFormat::Format_YUV420P :
			return QStringLiteral( "YUV420P" );

		case QVideoFrameFormat::Format_YUV422P :
			return QStringLiteral( "YUV422P" );

		case QVideoFrameFormat::Format_YV12 :
			return QStringLiteral( "YV12" );

		case QVideoFrameFormat::Format_UYVY :
			return QStringLiteral( "UYVY" );

		case QVideoFrameFormat::Format_YUYV :
			return QStringLiteral( "YUYV" );

		case QVideoFrameFormat::Format_NV12 :
			return QStringLiteral( "NV12" );

		case QVideoFrameFormat::Format_NV21 :
			return QStringLiteral( "NV21" );

		case QVideoFrameFormat::Format_IMC1 :
			return QStringLiteral( "IMC1" );

		case QVideoFrameFormat::Format_IMC2 :
			return QStringLiteral( "IMC2" );

		case QVideoFrameFormat::Format_IMC3 :
			return QStringLiteral( "IMC3" );

		case QVideoFrameFormat::Format_IMC4 :
			return QStringLiteral( "IMC4" );

		case QVideoFrameFormat::Format_Y8 :
			return QStringLiteral( "Y8" );

		case QVideoFrameFormat::Format_Y16 :
			return QStringLiteral( "Y16" );

		case QVideoFrameFormat::Format_P010 :
			return QStringLiteral( "P010" );

		case QVideoFrameFormat::Format_P016 :
			return QStringLiteral( "P016" );

		case QVideoFrameFormat::Format_Jpeg :
			return QStringLiteral( "JPEG" );

		case QVideoFrameFormat::Format_SamplerExternalOES :
			return QStringLiteral( "SEOES" );

		case QVideoFrameFormat::Format_SamplerRect :
			return QStringLiteral( "SR" );

		case QVideoFrameFormat::Format_Invalid :
			return QStringLiteral( "Invalid" );

		default :
			return QStringLiteral( "Unknown" );
	}
}

//
// stringToPixelFormat
//

//! \return String representation of pixel format.
QVideoFrameFormat::PixelFormat
stringToPixelFormat( const QString & s )
{
	if( s == QStringLiteral( "ARGB8888" ) )
		return QVideoFrameFormat::Format_ARGB8888;
	else if( s == QStringLiteral( "ARGB8888P" ) )
		return QVideoFrameFormat::Format_ARGB8888_Premultiplied;
	else if( s == QStringLiteral( "XRGB8888" ) )
		return QVideoFrameFormat::Format_XRGB8888;
	else if( s == QStringLiteral( "BGRA8888" ) )
		return QVideoFrameFormat::Format_BGRA8888;
	else if( s == QStringLiteral( "BGRA8888P" ) )
		return QVideoFrameFormat::Format_BGRA8888_Premultiplied;
	else if( s == QStringLiteral( "BGRX8888" ) )
		return QVideoFrameFormat::Format_BGRX8888;
	else if( s == QStringLiteral( "ARGB8888" ) )
		return QVideoFrameFormat::Format_ABGR8888;
	else if( s == QStringLiteral( "XBGR8888" ) )
		return QVideoFrameFormat::Format_XBGR8888;
	else if( s == QStringLiteral( "RGBA8888" ) )
		return QVideoFrameFormat::Format_RGBA8888;
	else if( s == QStringLiteral( "RGBX8888" ) )
		return QVideoFrameFormat::Format_RGBX8888;
	else if( s == QStringLiteral( "AYUV" ) )
		return QVideoFrameFormat::Format_AYUV;
	else if( s == QStringLiteral( "AYUVP" ) )
		return QVideoFrameFormat::Format_AYUV_Premultiplied;
	else if( s == QStringLiteral( "YUV420P" ) )
		return QVideoFrameFormat::Format_YUV420P;
	else if( s == QStringLiteral( "YUV422P" ) )
		return QVideoFrameFormat::Format_YUV422P;
	else if( s == QStringLiteral( "YV12" ) )
		return QVideoFrameFormat::Format_YV12;
	else if( s == QStringLiteral( "UYVY" ) )
		return QVideoFrameFormat::Format_UYVY;
	else if( s == QStringLiteral( "YUYV" ) )
		return QVideoFrameFormat::Format_YUYV;
	else if( s == QStringLiteral( "NV12" ) )
		return QVideoFrameFormat::Format_NV12;
	else if( s == QStringLiteral( "NV21" ) )
		return QVideoFrameFormat::Format_NV21;
	else if( s == QStringLiteral( "IMC1" ) )
		return QVideoFrameFormat::Format_IMC1;
	else if( s == QStringLiteral( "IMC2" ) )
		return QVideoFrameFormat::Format_IMC2;
	else if( s == QStringLiteral( "IMC3" ) )
		return QVideoFrameFormat::Format_IMC3;
	else if( s == QStringLiteral( "IMC4" ) )
		return QVideoFrameFormat::Format_IMC4;
	else if( s == QStringLiteral( "Y8" ) )
		return QVideoFrameFormat::Format_Y8;
	else if( s == QStringLiteral( "Y16" ) )
		return QVideoFrameFormat::Format_Y16;
	else if( s == QStringLiteral( "P010" ) )
		return QVideoFrameFormat::Format_P010;
	else if( s == QStringLiteral( "P016" ) )
		return QVideoFrameFormat::Format_P016;
	else if( s == QStringLiteral( "JPEG" ) )
		return QVideoFrameFormat::Format_Jpeg;
	else if( s == QStringLiteral( "SEOES" ) )
		return QVideoFrameFormat::Format_SamplerExternalOES;
	else if( s == QStringLiteral( "SR" ) )
		return QVideoFrameFormat::Format_SamplerRect;
	else
		return QVideoFrameFormat::Format_Invalid;
}


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

	bool first = true;

	QCameraFormat defaultFormat;

	for( auto it = m_camsInfo.cbegin(), last = m_camsInfo.cend(); it != last; ++it )
	{
		const auto settings = it.value().videoFormats();

		auto min = settings.first();

		for( const auto & s : settings )
		{
			m_resolutions[ it.key() ].push_back( s );

			if( first && s.resolution().width() < min.resolution().width() )
				min = s;
		}

		if( first )
			defaultFormat = min;

		first = false;
	}

	readCfg( defaultFormat );

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
		{
			for( const auto & s : qAsConst( it.value() ) )
			{
				if( s.resolution().width() == m_cfg.width() &&
					s.resolution().height() == m_cfg.height() &&
					qAbs( s.maxFrameRate() - m_cfg.frames() ) < 0.1 &&
					s.pixelFormat() == stringToPixelFormat( m_cfg.format() ) )
				{
					return s;
				}
			}
		}
	}

	return QCameraFormat();
}

QString
CameraSettings::camSettingsStr() const
{
	return resolution( camSettings() );
}

void
CameraSettings::setCamSettings( const QString & resolution, bool notify )
{
	static const QRegularExpression parser( QStringLiteral( "^(\\d+)x(\\d+)@(\\d+),(\\w+)$" ) );

	const auto match = parser.match( resolution );

	if( match.hasMatch() )
	{
		m_cfg.set_width( match.captured( 1 ).toInt() );
		m_cfg.set_height( match.captured( 2 ).toInt() );
		m_cfg.set_frames( match.captured( 3 ).toInt() );
		m_cfg.set_format( match.captured( 4 ) );

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
	m_cfg.set_format( pixelFormatToString( s.pixelFormat() ) );

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
			resolutions.append( resolution( s ) );

		m_camResolutions.setStringList( resolutions );

		m_dirty = true;

		if( notify )
			emit camSettingsChanged();
	}
}

QString
CameraSettings::resolution( const QCameraFormat & fmt ) const
{
	return QStringLiteral( "%1x%2@%3,%4" )
		.arg( QString::number( fmt.resolution().width() ),
			QString::number( fmt.resolution().height() ),
			QString::number( qRound( fmt.maxFrameRate() ) ),
			pixelFormatToString( fmt.pixelFormat() ) );
}

QString
CameraSettings::resolution() const
{
	return resolution( camSettings() );
}

void
CameraSettings::readCfg( const QCameraFormat & defaultFormat )
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

			return;
		}
		catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
		{
			file.close();
		}
	}

	setCamSettings( defaultFormat, false );
	setCamName( camName() );

	applySettings();
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
