
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
#include "mainwindow.hpp"
#include "view.hpp"
#include "config.hpp"
#include "options.hpp"
#include "server.hpp"
#include "shared/datagrams.hpp"
#include "shared/exceptions.hpp"
#include "db.hpp"
#include "by_product_model.hpp"
#include "db_signals.hpp"
#include "product.hpp"
#include "by_product_sort_model.hpp"
#include "by_product_view.hpp"
#include "by_place_view.hpp"
#include "by_place_model.hpp"
#include "by_place_sort_model.hpp"
#include "license_dialog.hpp"

// Qt include.
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QToolBar>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QHostAddress>
#include <QUdpSocket>
#include <QActionGroup>

// cfgfile include.
#include <cfgfile/all.hpp>


namespace Stock {

//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	MainWindowPrivate( MainWindow * parent )
		:	m_view( Q_NULLPTR )
		,	m_codeModel( Q_NULLPTR )
		,	m_placeModel( Q_NULLPTR )
		,	m_codeFilterModel( Q_NULLPTR )
		,	m_placeFilterModel( Q_NULLPTR )
		,	m_db( Q_NULLPTR )
		,	m_udp( Q_NULLPTR )
		,	m_srv( Q_NULLPTR )
		,	m_tray( Q_NULLPTR )
		,	m_sigs( Q_NULLPTR )
		,	q( parent )
		,	m_notifyOnOptionsChanges( true )
	{
	}

	~MainWindowPrivate()
	{
		if( m_udp )
			m_udp->close();

		if( m_srv )
			m_srv->close();
	}

	//! Init.
	void init();

	//! Tree view.
	View * m_view;
	//! By product model.
	ByProductModel * m_codeModel;
	//! By place model.
	ByPlaceModel * m_placeModel;
	//! By product filter model.
	ByProductSortModel * m_codeFilterModel;
	//! By place filter model.
	ByPlaceSortModel * m_placeFilterModel;
	//! Database.
	Db * m_db;
	//! UDP.
	QUdpSocket * m_udp;
	//! Server.
	Server * m_srv;
	//! System tray.
	QSystemTrayIcon * m_tray;
	//! DB signals.
	DbSignals * m_sigs;
	//! Parent.
	MainWindow * q;
	//! Notify on option changes?
	bool m_notifyOnOptionsChanges;
	//! Configuration.
	Cfg m_cfg;
}; // class MainWindowPrivate

void MainWindowPrivate::init()
{
	m_view = new View( q );

	m_sigs = new DbSignals( q );

	q->setCentralWidget( m_view );

	auto * file = q->menuBar()->addMenu( MainWindow::tr( "&File" ) );

	file->addAction( QIcon( ":/img/application-exit_22x22.png" ),
		MainWindow::tr( "&Quit" ), q, &MainWindow::quit,
		QKeySequence( "Ctrl+Q" ) );

	auto * edit = q->menuBar()->addMenu( MainWindow::tr( "&Edit" ) );

	auto * add = edit->addAction( QIcon( ":/img/list-add_22x22.png" ),
		MainWindow::tr( "Add Product and/or Place" ), q, &MainWindow::addProduct,
		QKeySequence( "Ctrl+=" ) );

	auto * opt = q->menuBar()->addMenu( MainWindow::tr( "&Options" ) );

	opt->addAction( QIcon( ":/img/configure_22x22.png" ),
		MainWindow::tr( "&Settings" ), q, &MainWindow::options,
		QKeySequence( "Ctrl+O" ) );

	auto * help = q->menuBar()->addMenu( MainWindow::tr( "&Help" ) );

	help->addAction( QIcon( ":/img/icon_22x22.png" ),
		MainWindow::tr( "&About" ), q, &MainWindow::about );
	help->addAction( QIcon( ":/img/qt.png" ),
			MainWindow::tr( "About Qt" ), q, &MainWindow::aboutQt );
	help->addAction( MainWindow::tr( "Licenses" ), q, &MainWindow::licenses );

	if( QSystemTrayIcon::isSystemTrayAvailable() )
	{
		m_tray = new QSystemTrayIcon( q );

		QMenu * menu = new QMenu( q );
		menu->addAction( QIcon(  ":/img/icon_22x22.png" ),
			MainWindow::tr( "Open Stock" ), q, &MainWindow::showWindow );
		menu->addSeparator();
		menu->addAction( QIcon( ":/img/application-exit_22x22.png" ),
			MainWindow::tr( "&Quit" ), q, &MainWindow::quit );

		m_tray->setContextMenu( menu );

		QIcon appIcon( ":/img/icon_256x256.png" );
		appIcon.addFile( ":/img/icon_128x128.png" );
		appIcon.addFile( ":/img/icon_64x64.png" );
		appIcon.addFile( ":/img/icon_48x48.png" );
		appIcon.addFile( ":/img/icon_32x32.png" );
		appIcon.addFile( ":/img/icon_22x22.png" );

		m_tray->setIcon( appIcon );
		m_tray->show();
	}

	auto * mode = new QToolBar( q );
	auto * modeGroup = new QActionGroup( mode );
	auto * code = new QAction( QIcon( ":/img/view-barcode_22x22.png" ),
		MainWindow::tr( "By Product Code" ), modeGroup );
	code->setActionGroup( modeGroup );
	code->setCheckable( true );
	code->setChecked( true );
	code->setShortcut( QKeySequence( "Ctrl+K" ) );

	QObject::connect( code, &QAction::triggered,
		q, &MainWindow::switchToByProductCode );

	auto * place = new QAction( QIcon( ":/img/flag-blue_22x22.png" ),
		MainWindow::tr( "By Place" ), modeGroup );
	place->setActionGroup( modeGroup );
	place->setCheckable( true );
	place->setChecked( false );
	place->setShortcut( QKeySequence( "Ctrl+P" ) );

	QObject::connect( place, &QAction::triggered,
		q, &MainWindow::switchToByPlace );

	mode->addAction( code );
	mode->addAction( place );

	q->addToolBar( mode );

	auto * actions = new QToolBar( q );
	actions->addAction( add );

	q->addToolBar( actions );

	QObject::connect( q, &MainWindow::started, q, &MainWindow::appStarted,
		Qt::QueuedConnection );

	emit q->started();
}


//
// MainWindow
//

MainWindow::MainWindow()
	:	d( new MainWindowPrivate( this ) )
{
	d->init();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::quit()
{
	QApplication::quit();
}

void
MainWindow::showWindow()
{
	show();

	setWindowState( ( windowState() & ~Qt::WindowMinimized ) | Qt::WindowActive );

	activateWindow();

	raise();
}

void
MainWindow::switchToByProductCode()
{
	d->m_view->switchToByProductCode();
}

void
MainWindow::switchToByPlace()
{
	d->m_view->switchToByPlace();
}

static const QString c_appCfgFileName = QLatin1String( "./config.cfg" );

void
MainWindow::appStarted()
{
	QFile file( c_appCfgFileName );

	if( file.exists() )
	{
		if( file.open( QIODevice::ReadOnly ) )
		{
			try {
				tag_Cfg< cfgfile::qstring_trait_t > tag;

				QTextStream stream( &file );

				cfgfile::read_cfgfile( tag, stream, c_appCfgFileName );

				file.close();

				d->m_cfg = tag.get_cfg();
			}
			catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
			{
				file.close();

				QMessageBox::critical( this, tr( "Couldn't read configuration..." ),
					tr( "Couldn't read configuration.\n\n%1" ).arg( x.desc() ) );

				QApplication::quit();
			}

			createDb();

			startNetwork();
		}
		else
		{
			QMessageBox::critical( this, tr( "Couldn't read configuration..." ),
				tr( "Couldn't open configuration file." ) );

			QApplication::quit();
		}
	}
	else
	{
		d->m_notifyOnOptionsChanges = false;

		options();

		if( d->m_cfg.host().isEmpty() )
			QApplication::quit();

		createDb();

		startNetwork();
	}
}

void
MainWindow::createDb()
{
	try {
		d->m_db = new Db( this );

		d->m_codeModel = new ByProductModel( this, d->m_db,
			d->m_sigs, this );
		d->m_placeModel = new ByPlaceModel( this, d->m_db,
			d->m_sigs, this );

		d->m_codeFilterModel = new ByProductSortModel( this );
		d->m_codeFilterModel->setSourceModel( d->m_codeModel );

		d->m_placeFilterModel = new ByPlaceSortModel( this );
		d->m_placeFilterModel->setSourceModel( d->m_placeModel );

		d->m_view->setModels( d->m_codeModel, d->m_placeModel );
		d->m_view->setFilterModels( d->m_codeFilterModel, d->m_placeFilterModel );

		d->m_view->setDb( d->m_sigs, d->m_db );
	}
	catch( const Exception & x )
	{
		QMessageBox::critical( this, tr( "Couldn't open database..." ),
			tr( "Couldn't open database.\n\n%1" ).arg( x.msg() ) );

		QApplication::quit();
	}
}

void
MainWindow::startNetwork()
{
	QHostAddress host;

	if( host.setAddress( d->m_cfg.host() ) )
	{
		d->m_srv = new Server( this );
		d->m_srv->setSecret( d->m_cfg.secret() );

		if( !d->m_srv->listen( host, d->m_cfg.port() ) )
			cantStartNetwork( d->m_srv->errorString() );

		d->m_srv->setDbAndModels( d->m_db, d->m_sigs,
			d->m_codeModel, d->m_placeModel );

		d->m_udp = new QUdpSocket( this );

		if( !d->m_udp->bind( c_udpPort, QUdpSocket::ShareAddress ) )
			cantStartNetwork( d->m_udp->errorString() );

		connect( d->m_udp, &QUdpSocket::readyRead,
			this, &MainWindow::readPendingDatagrams );
	}
	else
	{
		QFile file( c_appCfgFileName );
		file.remove();

		QMessageBox::critical( this, tr( "Incorrect host..." ),
			tr( "Please configure application. "
				"Host address is incorrect: \"%1\"" ).arg( d->m_cfg.host() ) );

		QApplication::quit();
	}
}

void
MainWindow::cantStartNetwork( const QString & error )
{
	QMessageBox::critical( this, tr( "Application already started..." ),
		tr( "Seems that application is already started. "
			"But possibly that port %1 is in use by another "
			"application, in such case, please, reconfigure "
			"application. Possibly application can't start if options are "
			"wrong, in such case, please, reconfigure application too. "
			"To reconfigure application, please, edit \"%2\" file in "
			"installation directory or just delete this file.\n\n%3" )
				.arg( QString::number( d->m_cfg.port() ) )
				.arg( c_appCfgFileName )
				.arg( error ) );

	QApplication::quit();
}

void
MainWindow::readPendingDatagrams()
{
	while( d->m_udp->hasPendingDatagrams() )
	{
		QNetworkDatagram datagram = d->m_udp->receiveDatagram();

		if( datagramType( datagram ) == DatagramType::TellIP )
		{
			Messages::TellMeYourIP msg;

			try {
				readDatagram< Messages::TellMeYourIP,
					Messages::tag_TellMeYourIP< cfgfile::qstring_trait_t > > (
						datagram, msg );

				writeMyIpDatargam( d->m_udp, d->m_cfg.host(), d->m_cfg.port(),
					datagram.senderAddress(), datagram.senderPort() );
			}
			catch( const Exception & )
			{
			}
		}
	}
}

void
MainWindow::options()
{
	Options opt( d->m_cfg.host(), d->m_cfg.port(), d->m_cfg.secret(), this );

	if( opt.exec() == QDialog::Accepted )
	{
		d->m_cfg.set_host( opt.host() );
		d->m_cfg.set_port( opt.port() );
		d->m_cfg.set_secret( opt.password() );

		if( d->m_srv )
			d->m_srv->setSecret( opt.password() );

		QFile file( c_appCfgFileName );

		if( file.open( QIODevice::WriteOnly ) )
		{
			try {
				tag_Cfg< cfgfile::qstring_trait_t > tag( d->m_cfg );

				QTextStream stream( &file );

				cfgfile::write_cfgfile( tag, stream );

				file.close();
			}
			catch( const cfgfile::exception_t< cfgfile::qstring_trait_t > & x )
			{
				file.close();

				d->m_notifyOnOptionsChanges = false;

				QMessageBox::critical( this, tr( "Couldn't write configuration..." ),
					tr( "Couldn't write configuration.\n\n%1" ).arg( x.desc() ) );
			}
		}
		else
		{
			d->m_notifyOnOptionsChanges = false;

			QMessageBox::critical( this, tr( "Couldn't write configuration..." ),
				tr( "Couldn't open configuration file for writting." ) );
		}

		if( d->m_notifyOnOptionsChanges )
			QMessageBox::information( this, tr( "Settings..." ),
				tr( "New settings will be applied after restart of the "
					"application." ) );
	}

	d->m_notifyOnOptionsChanges = true;
}

void
MainWindow::about()
{
	QMessageBox::about( this, tr( "About Stock" ),
		tr( "Stock - GUI warehouse accounting application.\n\n"
			"Author - Igor Mironchik (igor.mironchik at gmail dot com).\n\n"
			"Copyright (c) 2018 Igor Mironchik.\n\n"
			"Licensed under GNU GPL 3.0." ) );
}

void
MainWindow::aboutQt()
{
	QMessageBox::aboutQt( this );
}

void
MainWindow::licenses()
{
	LicenseDialog msg( this );
	msg.addLicense( QStringLiteral( "<p><b>The Oxygen Icon Theme</b>\n\n</p>"
		"<p>Copyright (C) 2007 Nuno Pinheiro &lt;nuno@oxygen-icons.org&gt;\n</p>"
		"<p>Copyright (C) 2007 David Vignoni &lt;david@icon-king.com&gt;\n</p>"
		"<p>Copyright (C) 2007 David Miller &lt;miller@oxygen-icons.org&gt;\n</p>"
		"<p>Copyright (C) 2007 Johann Ollivier Lapeyre &lt;johann@oxygen-icons.org&gt;\n</p>"
		"<p>Copyright (C) 2007 Kenneth Wimer &lt;kwwii@bootsplash.org&gt;\n</p>"
		"<p>Copyright (C) 2007 Riccardo Iaconelli &lt;riccardo@oxygen-icons.org&gt;\n</p>"
		"<p>\nand others\n</p>"
		"\n"
		"<p>This library is free software; you can redistribute it and/or "
		"modify it under the terms of the GNU Lesser General Public "
		"License as published by the Free Software Foundation; either "
		"version 3 of the License, or (at your option) any later version.\n</p>"
		"\n"
		"<p>This library is distributed in the hope that it will be useful, "
		"but WITHOUT ANY WARRANTY; without even the implied warranty of "
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU "
		"Lesser General Public License for more details.\n</p>"
		"\n"
		"<p>You should have received a copy of the GNU Lesser General Public "
		"License along with this library. If not, see "
		"<a href=\"http://www.gnu.org/licenses/\">&lt;http://www.gnu.org/licenses/&gt;</a>.\n</p>"
		"\n"
		"<p>Clarification:\n</p>"
		"\n"
		"<p>The GNU Lesser General Public License or LGPL is written for "
		"software libraries in the first place. We expressly want the LGPL to "
		"be valid for this artwork library too.\n</p>"
		"\n"
		"<p>KDE Oxygen theme icons is a special kind of software library, it is an "
		"artwork library, it's elements can be used in a Graphical User Interface, or "
		"GUI.\n</p>"
		"\n"
		"<p>Source code, for this library means:\n</p>"
		"<p><ul> <li>where they exist, SVG;\n</li>"
		" <li>otherwise, if applicable, the multi-layered formats xcf or psd, or "
		"otherwise png.\n</li></ul></p>"
		"\n"
		"<p>The LGPL in some sections obliges you to make the files carry "
		"notices. With images this is in some cases impossible or hardly useful.\n</p>"
		"\n"
		"<p>With this library a notice is placed at a prominent place in the directory "
		"containing the elements. You may follow this practice.\n</p>"
		"\n"
		"<p>The exception in section 5 of the GNU Lesser General Public License covers "
		"the use of elements of this art library in a GUI.\n</p>"
		"\n"
		"<p>kde-artists [at] kde.org\n</p>"
		"\n"
		"<p><b>GNU LESSER GENERAL PUBLIC LICENSE</b>\n</p>"
		"<p>Version 3, 29 June 2007\n</p>"
		"\n"
		"<p>Copyright (C) 2007 Free Software Foundation, Inc. <a href=\"http://fsf.org/\">&lt;http://fsf.org/&gt;</a> "
		"Everyone is permitted to copy and distribute verbatim copies "
		"of this license document, but changing it is not allowed.\n</p>"
		"\n"
		"\n"
		"<p>This version of the GNU Lesser General Public License incorporates "
		"the terms and conditions of version 3 of the GNU General Public "
		"License, supplemented by the additional permissions listed below.\n</p>"
		"\n"
		"<p><b>0.</b> Additional Definitions.\n</p>"
		"\n"
		"<p>As used herein, \"this License\" refers to version 3 of the GNU Lesser "
		"General Public License, and the \"GNU GPL\" refers to version 3 of the GNU "
		"General Public License.\n</p>"
		"\n"
		"<p>\"The Library\" refers to a covered work governed by this License, "
		"other than an Application or a Combined Work as defined below.\n</p>"
		"\n"
		"<p>An \"Application\" is any work that makes use of an interface provided "
		"by the Library, but which is not otherwise based on the Library. "
		"Defining a subclass of a class defined by the Library is deemed a mode "
		"of using an interface provided by the Library.\n</p>"
		"\n"
		"<p>A \"Combined Work\" is a work produced by combining or linking an "
		"Application with the Library.  The particular version of the Library "
		"with which the Combined Work was made is also called the \"Linked "
		"Version\".\n</p>"
		"\n"
		"<p>The \"Minimal Corresponding Source\" for a Combined Work means the "
		"Corresponding Source for the Combined Work, excluding any source code "
		"for portions of the Combined Work that, considered in isolation, are "
		"based on the Application, and not on the Linked Version.\n</p>"
		"\n"
		"<p>The \"Corresponding Application Code\" for a Combined Work means the "
		"object code and/or source code for the Application, including any data "
		"and utility programs needed for reproducing the Combined Work from the "
		"Application, but excluding the System Libraries of the Combined Work.\n</p>"
		"\n"
		"<p><b>1.</b> Exception to Section 3 of the GNU GPL.\n</p>"
		"\n"
		"<p>You may convey a covered work under sections 3 and 4 of this License "
		"without being bound by section 3 of the GNU GPL.\n</p>"
		"\n"
		"<p><b>2.</b> Conveying Modified Versions.\n</p>"
		"\n"
		"<p>If you modify a copy of the Library, and, in your modifications, a "
		"facility refers to a function or data to be supplied by an Application "
		"that uses the facility (other than as an argument passed when the "
		"facility is invoked), then you may convey a copy of the modified "
		"version:\n</p>"
		"\n"
		"<p><b>a)</b> under this License, provided that you make a good faith effort to "
		"ensure that, in the event an Application does not supply the "
		"function or data, the facility still operates, and performs "
		"whatever part of its purpose remains meaningful, or\n</p>"
		"\n"
		"<p><b>b)</b> under the GNU GPL, with none of the additional permissions of "
		"this License applicable to that copy.\n</p>"
		"\n"
		"<p><b>3.</b> Object Code Incorporating Material from Library Header Files.\n</p>"
		"\n"
		"<p>The object code form of an Application may incorporate material from "
		"a header file that is part of the Library.  You may convey such object "
		"code under terms of your choice, provided that, if the incorporated "
		"material is not limited to numerical parameters, data structure "
		"layouts and accessors, or small macros, inline functions and templates "
		"(ten or fewer lines in length), you do both of the following:\n</p>"
		"\n"
		"<p><b>a)</b> Give prominent notice with each copy of the object code that the "
		"Library is used in it and that the Library and its use are "
		"covered by this License.\n</p>"
		"\n"
		"<p><b>b)</b> Accompany the object code with a copy of the GNU GPL and this license "
		"document.\n</p>"
		"\n"
		"<p><b>4.</b> Combined Works.\n</p>"
		"\n"
		"<p>You may convey a Combined Work under terms of your choice that, "
		"taken together, effectively do not restrict modification of the "
		"portions of the Library contained in the Combined Work and reverse "
		"engineering for debugging such modifications, if you also do each of "
		"the following:\n</p>"
		"\n"
		"<p><b>a)</b> Give prominent notice with each copy of the Combined Work that "
		"the Library is used in it and that the Library and its use are "
		"covered by this License.\n</p>"
		"\n"
		"<p><b>b)</b> Accompany the Combined Work with a copy of the GNU GPL and this license "
		"document.\n</p>"
		"\n"
		"<p><b>c)</b> For a Combined Work that displays copyright notices during "
		"execution, include the copyright notice for the Library among "
		"these notices, as well as a reference directing the user to the "
		"copies of the GNU GPL and this license document.\n</p>"
		"\n"
		"<p><b>d)</b> Do one of the following:\n</p>"
		"\n"
		"<p>    <b>0)</b> Convey the Minimal Corresponding Source under the terms of this "
		"License, and the Corresponding Application Code in a form "
		"suitable for, and under terms that permit, the user to "
		"recombine or relink the Application with a modified version of "
		"the Linked Version to produce a modified Combined Work, in the "
		"manner specified by section 6 of the GNU GPL for conveying "
		"Corresponding Source.\n</p>"
		"\n"
		"<p>    <b>1)</b> Use a suitable shared library mechanism for linking with the "
		"Library.  A suitable mechanism is one that (a) uses at run time "
		"a copy of the Library already present on the user's computer "
		"system, and (b) will operate properly with a modified version "
		"of the Library that is interface-compatible with the Linked "
		"Version.\n</p>"
		"\n"
		"<p><b>e)</b> Provide Installation Information, but only if you would otherwise "
		"be required to provide such information under section 6 of the "
		"GNU GPL, and only to the extent that such information is "
		"necessary to install and execute a modified version of the "
		"Combined Work produced by recombining or relinking the "
		"Application with a modified version of the Linked Version. (If "
		"you use option 4d0, the Installation Information must accompany "
		"the Minimal Corresponding Source and Corresponding Application "
		"Code. If you use option 4d1, you must provide the Installation "
		"Information in the manner specified by section 6 of the GNU GPL "
		"for conveying Corresponding Source.)\n</p>"
		"\n"
		"<p><b>5.</b> Combined Libraries.\n</p>"
		"\n"
		"<p>You may place library facilities that are a work based on the "
		"Library side by side in a single library together with other library "
		"facilities that are not Applications and are not covered by this "
		"License, and convey such a combined library under terms of your "
		"choice, if you do both of the following:\n</p>"
		"\n"
		"<p><b>a)</b> Accompany the combined library with a copy of the same work based "
		"on the Library, uncombined with any other library facilities, "
		"conveyed under the terms of this License.\n</p>"
		"\n"
		"<p><b>b)</b> Give prominent notice with the combined library that part of it "
		"is a work based on the Library, and explaining where to find the "
		"accompanying uncombined form of the same work.\n</p>"
		"\n"
		"<p><b>6.</b> Revised Versions of the GNU Lesser General Public License.\n</p>"
		"\n"
		"<p>The Free Software Foundation may publish revised and/or new versions "
		"of the GNU Lesser General Public License from time to time. Such new "
		"versions will be similar in spirit to the present version, but may "
		"differ in detail to address new problems or concerns.\n</p>"
		"\n"
		"<p>Each version is given a distinguishing version number. If the "
		"Library as you received it specifies that a certain numbered version "
		"of the GNU Lesser General Public License \"or any later version\" "
		"applies to it, you have the option of following the terms and "
		"conditions either of that published version or of any later version "
		"published by the Free Software Foundation. If the Library as you "
		"received it does not specify a version number of the GNU Lesser "
		"General Public License, you may choose any version of the GNU Lesser "
		"General Public License ever published by the Free Software Foundation.\n</p>"
		"\n"
		"<p>If the Library as you received it specifies that a proxy can decide "
		"whether future versions of the GNU Lesser General Public License shall "
		"apply, that proxy's public statement of acceptance of any version is "
		"permanent authorization for you to choose that version for the "
		"Library.</p>" ) );

	msg.addLicense( QStringLiteral( "<p><b>easy-encryption License</b>\n\n</p>"
		"<p>MIT License\n</p>"
		"\n"
		"<p>Copyright (c) 2018 Philippe Rémy\n</p>"
		"\n"
		"<p>Permission is hereby granted, free of charge, to any person obtaining a copy "
		"of this software and associated documentation files (the \"Software\"), to deal "
		"in the Software without restriction, including without limitation the rights "
		"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
		"copies of the Software, and to permit persons to whom the Software is "
		"furnished to do so, subject to the following conditions:\n</p>"
		"\n"
		"<p>The above copyright notice and this permission notice shall be included in all "
		"copies or substantial portions of the Software.\n</p>"
		"\n"
		"<p>THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
		"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
		"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
		"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
		"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
		"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
		"SOFTWARE.</p>" ) );

	msg.exec();
}

void
MainWindow::addProduct()
{
	ProductDlg dlg( QString(), QString(), 0, QString(),
		d->m_codeModel->codes(), d->m_placeModel->places(),
		true, d->m_codeModel, this );

	if( dlg.exec() == QDialog::Accepted )
	{
		DbResult res = d->m_db->changeProduct( { dlg.code(),
			dlg.place(), dlg.count(), dlg.desc() } );

		if( res.m_ok )
			d->m_sigs->emitProductChanged( dlg.code(), dlg.place(),
				dlg.count(), dlg.desc() );
		else
			QMessageBox::critical( this, tr( "Error in the database..." ),
				tr( "Unable to add the product.\n\n%1" )
					.arg( res.m_error ) );
	}
}

void
MainWindow::closeEvent( QCloseEvent * e )
{
	e->ignore();

	if( d->m_tray )
		hide();
	else
		showMinimized();
}

} /* namespace Stock */
