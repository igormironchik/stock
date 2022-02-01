
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
