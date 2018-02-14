
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

#ifndef STOCK_MAINWINDOW_HPP_INCLUDED
#define STOCK_MAINWINDOW_HPP_INCLUDED

// Qt include.
#include <QMainWindow>
#include <QScopedPointer>


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

private:
	//! Start network.
	void startNetwork();
	//! Can't start network.
	void cantStartNetwork( const QString & error );

protected:
	void closeEvent( QCloseEvent * e ) Q_DECL_OVERRIDE;

private:
	friend class MainWindowPrivate;

	Q_DISABLE_COPY( MainWindow )

	QScopedPointer< MainWindowPrivate > d;
}; // class MainWindow

} /* namespace Stock */

#endif // STOCK_MAINWINDOW_HPP_INCLUDED
