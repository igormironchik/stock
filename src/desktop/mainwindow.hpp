
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_MAINWINDOW_HPP_INCLUDED
#define STOCK_MAINWINDOW_HPP_INCLUDED

// Qt include.
#include <QMainWindow>

// C++ include.
#include <memory>


namespace Stock {

//
// MainWindow
//

class MainWindowPrivate;

//! Main window.
class MainWindow Q_DECL_FINAL
	:	public QMainWindow
{
	Q_OBJECT

signals:
	//! Started.
	void started();

public:
	MainWindow();
	virtual ~MainWindow();

private slots:
	//! Quit.
	void quit();
	//! Show.
	void showWindow();
	//! Switch to "By Product Code" mode.
	void switchToByProductCode();
	//! Switch to "By place" mode.
	void switchToByPlace();
	//! Application started.
	void appStarted();
	//! Options.
	void options();
	//! Read UDP datagrams.
	void readPendingDatagrams();
	//! About.
	void about();
	//! About Qt.
	void aboutQt();
	//! Licenses.
	void licenses();
	//! Add product.
	void addProduct();

private:
	//! Start network.
	void startNetwork();
	//! Can't start network.
	void cantStartNetwork( const QString & error );
	//! Create DB.
	void createDb();

protected:
	void closeEvent( QCloseEvent * e ) Q_DECL_OVERRIDE;

private:
	friend class MainWindowPrivate;

	Q_DISABLE_COPY( MainWindow )

	std::unique_ptr< MainWindowPrivate > d;
}; // class MainWindow

} /* namespace Stock */

#endif // STOCK_MAINWINDOW_HPP_INCLUDED
