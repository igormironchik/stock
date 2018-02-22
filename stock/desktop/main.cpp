
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
	appTranslator.load( "./tr/stock_" + QLocale::system().name() );
	app.installTranslator( &appTranslator );

	Stock::MainWindow w;
	w.setMinimumSize( 650, 550 );
	w.showMaximized();

	return app.exec();
}
