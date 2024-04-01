
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "cfg_file.hpp"
#include "config.hpp"

// cfgfile include.
#include <cfgfile/all.hpp>

// Qt include.
#include <QTextStream>
#include <QFile>
#include <QDir>


namespace Stock {

//
// CfgFile
//

bool
CfgFile::read( const QString & fileName, QString & pwd, quint16 & port )
{
	QFile file( fileName );

	if( file.open( QIODevice::ReadOnly ) )
	{
		QTextStream s( &file );

		try {
			tag_Cfg< cfgfile::qstring_trait_t> tag;

			cfgfile::read_cfgfile( tag, s, fileName );

			pwd = tag.get_cfg().secret();
			port = tag.get_cfg().port();

			file.close();

			return true;
		}
		catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & )
		{
			file.close();

			return false;
		}
	}
	else
		return false;
}

void
CfgFile::write( const QString & fileName, const QString & pwd, quint16 port )
{
	QFile file( fileName );

	if( file.open( QIODevice::WriteOnly ) )
	{
		QTextStream s( &file );

		try {
			Cfg cfg;
			cfg.set_secret( pwd );
			cfg.set_port( port );
			tag_Cfg< cfgfile::qstring_trait_t> tag( cfg );

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
CfgFile::fileName()
{
	const auto configsPath = QLatin1String( "./Stock" );

	QDir dir( configsPath );

	if( !dir.exists() )
		dir.mkpath( QLatin1String( "." ) );

	return ( configsPath + QLatin1String( "/stock.cfg" ) );
}

} /* namespace Stock */
