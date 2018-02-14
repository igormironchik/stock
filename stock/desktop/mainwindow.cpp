
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
#include "mainwindow.hpp"
#include "view.hpp"
#include "config.hpp"
#include "options.hpp"
#include "server.hpp"
#include "shared/datagrams.hpp"
#include "shared/exceptions.hpp"
#include "db.hpp"
#include "by_product_model.hpp"

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
#include <QTextCodec>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTreeView>

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
		,	m_db( Q_NULLPTR )
		,	m_udp( Q_NULLPTR )
		,	m_srv( Q_NULLPTR )
		,	m_tray( Q_NULLPTR )
		,	m_notifyOnOptionsChanges( true )
		,	q( parent )
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
	//! Database.
	QScopedPointer< Db > m_db;
	//! UDP.
	QUdpSocket * m_udp;
	//! Server.
	Server * m_srv;
	//! System tray.
	QSystemTrayIcon * m_tray;
	//! Configuration.
	Cfg m_cfg;
	//! Notify on option changes?
	bool m_notifyOnOptionsChanges;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

void MainWindowPrivate::init()
{
	m_view = new View( q );

	q->setCentralWidget( m_view );

	auto * file = q->menuBar()->addMenu( MainWindow::tr( "&File" ) );

	file->addAction( QIcon( ":/img/application-exit_22x22.png" ),
		MainWindow::tr( "&Quit" ), q, &MainWindow::quit,
		QKeySequence( "Ctrl+Q" ) );

	auto * opt = q->menuBar()->addMenu( MainWindow::tr( "&Options" ) );

	opt->addAction( QIcon( ":/img/configure_22x22.png" ),
		MainWindow::tr( "&Settings" ), q, &MainWindow::options,
		QKeySequence( "Ctrl+O" ) );

	auto * help = q->menuBar()->addMenu( MainWindow::tr( "&Help" ) );

	help->addAction( QIcon( ":/img/icon_22x22.png" ),
		MainWindow::tr( "&About" ), q, &MainWindow::about );
	help->addAction( QIcon( ":/img/qt.png" ),
			MainWindow::tr( "About Qt" ), q, &MainWindow::aboutQt );

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
	auto * code = new QAction( QIcon( ":/img/feed-subscribe_22x22.png" ),
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
	setWindowState( ( windowState() & ~Qt::WindowMinimized) | Qt::WindowActive );

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
				stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

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

			try {
				d->m_db.reset( new Db );

				d->m_codeModel = new ByProductModel( d->m_db.data(), this );

				d->m_view->byProductsView()->setModel( d->m_codeModel );
			}
			catch( const Exception & x )
			{
				QMessageBox::critical( this, tr( "Couldn't open database..." ),
					tr( "Couldn't open database.\n\n%1" ).arg( x.msg() ) );

				QApplication::quit();
			}

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

		startNetwork();
	}
}

void
MainWindow::startNetwork()
{
	QHostAddress host;

	if( host.setAddress( d->m_cfg.host() ) )
	{
		d->m_udp = new QUdpSocket( this );

		if( !d->m_udp->bind( host, d->m_cfg.port() ) )
			cantStartNetwork( d->m_udp->errorString() );

		connect( d->m_udp, &QUdpSocket::readyRead,
			this, &MainWindow::readPendingDatagrams );

		d->m_srv = new Server( this );

		if( !d->m_srv->listen( host, d->m_cfg.port() ) )
			cantStartNetwork( d->m_srv->errorString() );
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
			writeMyIpDatargam( d->m_udp, d->m_cfg.host(), d->m_cfg.port(),
				datagram.senderAddress(), datagram.senderPort() );
	}
}

void
MainWindow::options()
{
	Options opt( d->m_cfg.host(), d->m_cfg.port(), this );

	if( opt.exec() == QDialog::Accepted )
	{
		d->m_cfg.set_host( opt.host() );
		d->m_cfg.set_port( opt.port() );

		QFile file( c_appCfgFileName );

		if( file.open( QIODevice::WriteOnly ) )
		{
			try {
				tag_Cfg< cfgfile::qstring_trait_t > tag( d->m_cfg );

				QTextStream stream( &file );
				stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

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
MainWindow::closeEvent( QCloseEvent * e )
{
	e->ignore();

	if( d->m_tray )
		hide();
	else
		showMinimized();
}

} /* namespace Stock */
