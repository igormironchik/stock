
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
#include "camera_settings.hpp"

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
	if( appTranslator.load( "stock_" + QLocale::system().name(), "assets:/" ) )
#else
	if( appTranslator.load( "./tr/stock_" + QLocale::system().name() ) )
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

	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty( "passwordSet", passwordSet );
	engine.rootContext()->setContextProperty( "defaultUdpPort", port );
	engine.rootContext()->setContextProperty( "password", password );
	engine.rootContext()->setContextProperty( "qmlCppSignals", &sigs );
	engine.rootContext()->setContextProperty( "codesModel", sigs.codesModel() );
	engine.rootContext()->setContextProperty( "placesModel", sigs.placesModel() );
	engine.rootContext()->setContextProperty( "searchModel", sigs.searchModel() );
	engine.rootContext()->setContextProperty( "cameraSettings",
		&Stock::CameraSettings::instance() );

	engine.rootContext()->setContextProperty( "cameraModel",
		Stock::CameraSettings::instance().camsList() );
	engine.rootContext()->setContextProperty( "resolutionModel",
		Stock::CameraSettings::instance().camResolutions() );

	engine.load( QUrl( "qrc:/qml/main.qml" ) );

	if( engine.rootObjects().isEmpty() )
		return -1;

	return app.exec();
}
