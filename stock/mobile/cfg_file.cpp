
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018 Igor Mironchik

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
#include <QTextCodec>
#include <QFile>
#include <QStandardPaths>
#include <QDir>


namespace Stock {

static const QString c_configFoler = QLatin1String( "Stock" );

//
// CfgFile
//

bool
CfgFile::read( const QString & fileName, QString & pwd )
{
	QFile file( fileName );

	if( file.open( QIODevice::ReadOnly ) )
	{
		QTextStream s( &file );
		s.setCodec( QTextCodec::codecForName( "UTF-8" ) );

		try {
			tag_Cfg< cfgfile::qstring_trait_t> tag;

			cfgfile::read_cfgfile( tag, s, fileName );

			pwd = tag.get_cfg().secret();

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
CfgFile::write( const QString & fileName, const QString & pwd )
{
	QFile file( fileName );

	if( file.open( QIODevice::WriteOnly ) )
	{
		QTextStream s( &file );
		s.setCodec( QTextCodec::codecForName( "UTF-8" ) );

		try {
			Cfg cfg;
			cfg.set_secret( pwd );
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
	const auto configsPath = QStandardPaths::standardLocations(
		QStandardPaths::ConfigLocation ).first();

	QDir dir( configsPath );

	if( !dir.exists( c_configFoler ) )
		dir.mkdir( c_configFoler );

	return ( configsPath + QLatin1String( "/" ) + c_configFoler +
		QLatin1String( "/stock.cfg" ) );
}

} /* namespace Stock */
