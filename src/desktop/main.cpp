
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Qt include.
#include <QApplication>
#include <QTranslator>
#include <QLocale>

// Stock include.
#include "mainwindow.hpp"
#include "messages.hpp"


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

	QApplication app( argc, argv );

	app.setApplicationDisplayName( QObject::tr( "Stock" ) );
	app.setApplicationName( QObject::tr( "Stock" ) );
	app.setApplicationVersion( QLatin1String( "1.0" ) );

	QIcon appIcon( ":/img/icon_256x256.png" );
	appIcon.addFile( ":/img/icon_128x128.png" );
	appIcon.addFile( ":/img/icon_64x64.png" );
	appIcon.addFile( ":/img/icon_48x48.png" );
	appIcon.addFile( ":/img/icon_32x32.png" );
	appIcon.addFile( ":/img/icon_22x22.png" );
	app.setWindowIcon( appIcon );

	QTranslator appTranslator;
	if( appTranslator.load( "./tr/stock_" + QLocale::system().name() ) )
		app.installTranslator( &appTranslator );

	Stock::MainWindow w;
	w.setMinimumSize( 650, 550 );
	w.showMaximized();

	return app.exec();
}
