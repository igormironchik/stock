
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

// Qt include.
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QTranslator>
#include <QLocale>
#include <QQmlContext>
#include <QFileInfo>

// Stock include.
#include "qml_cpp_bridge.hpp"
#include "messages.hpp"
#include "cfg_file.hpp"


int main( int argc, char ** argv )
{
	qRegisterMetaType< Stock::Messages::AddProduct >(
		"Stock::Messages::AddProduct" );
	qRegisterMetaType< Stock::Messages::GiveListOfProducts >(
		"Stock::Messages::GiveListOfProducts" );
	qRegisterMetaType< Stock::Messages::ListOfProducts >(
		"Stock::Messages::ListOfProducts" );
	qRegisterMetaType< Stock::Messages::Error >(
		"Stock::Messages::Error" );
	qRegisterMetaType< Stock::Messages::Hello >(
		"Stock::Messages::Hello" );
	qRegisterMetaType< Stock::Messages::Ok >(
		"Stock::Messages::Ok" );

	QGuiApplication app( argc, argv );

	QIcon appIcon( ":/img/icon_256x256.png" );
	appIcon.addFile( ":/img/icon_128x128.png" );
	appIcon.addFile( ":/img/icon_64x64.png" );
	app.setWindowIcon( appIcon );

	QTranslator appTranslator;
	appTranslator.load( "./tr/stock_" + QLocale::system().name() );
	app.installTranslator( &appTranslator );

	const auto cfgFileName = Stock::CfgFile::fileName();

	bool passwordSet = false;
	QString password;

	if( QFileInfo::exists( cfgFileName ) )
	{
		if( !Stock::CfgFile::read( cfgFileName, password ) )
			password.clear();
		else
			passwordSet = true;
	}

	Stock::QmlCppBridge sigs( cfgFileName );

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty( "passwordSet", passwordSet );
	engine.rootContext()->setContextProperty( "password", password );
	engine.rootContext()->setContextProperty( "qmlCppSignals", &sigs );
	engine.load( QUrl( "qrc:/qml/main.qml" ) );

	if( engine.rootObjects().isEmpty() )
		return -1;

	return app.exec();
}
