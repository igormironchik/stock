
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

// Qt include.
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QTranslator>
#include <QLocale>
#include <QQmlContext>
#include <QFileInfo>
#include <QSortFilterProxyModel>

// Stock include.
#include "qml_cpp_bridge.hpp"
#include "messages.hpp"
#include "cfg_file.hpp"
#include "list_model.hpp"
#include "shared/constants.hpp"
#include "frames.hpp"
#include "image.hpp"

#include <QStringListModel>


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

	Stock::Frames::registerQmlType();
	Stock::PreviewImage::registerQmlType();

	QGuiApplication app( argc, argv );

	QIcon appIcon( ":/img/icon_256x256.png" );
	appIcon.addFile( ":/img/icon_128x128.png" );
	appIcon.addFile( ":/img/icon_64x64.png" );
	app.setWindowIcon( appIcon );

	QTranslator appTranslator;

#ifdef Q_OS_ANDROID
    appTranslator.load( "stock_" + QLocale::system().name(), "assets:/" );
#else
	appTranslator.load( "./tr/stock_" + QLocale::system().name() );
#endif

    app.installTranslator( &appTranslator );

    const auto cfgFileName = Stock::CfgFile::fileName();

	bool passwordSet = false;
	QString password;
	quint16 port = Stock::c_udpPort;

    if( QFileInfo::exists( cfgFileName ) )
    {
        if( !Stock::CfgFile::read( cfgFileName, password, port ) )
            password.clear();
        else
            passwordSet = true;
    }

	Stock::QmlCppBridge sigs( cfgFileName );
	QStringListModel cameraModel;
	QStringListModel resolutionModel;

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty( "passwordSet", passwordSet );
	engine.rootContext()->setContextProperty( "defaultUdpPort", port );
	engine.rootContext()->setContextProperty( "password", password );
	engine.rootContext()->setContextProperty( "qmlCppSignals", &sigs );
	engine.rootContext()->setContextProperty( "codesModel", sigs.codesModel() );
	engine.rootContext()->setContextProperty( "placesModel", sigs.placesModel() );
	engine.rootContext()->setContextProperty( "searchModel", sigs.searchModel() );

	engine.rootContext()->setContextProperty( "cameraModel", &cameraModel );
	engine.rootContext()->setContextProperty( "resolutionModel", &resolutionModel );

	engine.load( QUrl( "qrc:/qml/main.qml" ) );

	if( engine.rootObjects().isEmpty() )
		return -1;

	return app.exec();
}
