
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

#ifndef STOCK_VIEW_HPP_INCLUDED
#define STOCK_VIEW_HPP_INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace Stock {

//
// View
//

class ViewPrivate;

//! Central widget.
class View Q_DECL_FINAL
	:	public QWidget
{
	Q_OBJECT

public:
	explicit View( QWidget * parent );
	virtual ~View();

public slots:
	//! Switch to by product code mode.
	void switchToByProductCode();
	//! Switch to by place mode.
	void switchToByPlace();

private slots:
	//! Product code changed.
	void productCodeChanged( const QString & txt );
	//! Name of product changed.
	void nameOfProductChanged( const QString & txt );
	//! Place changed.
	void placeChanged( const QString & txt );

private:
	friend class ViewPrivate;

	Q_DISABLE_COPY( View )

	QScopedPointer< ViewPrivate > d;
}; // class View

} /* namespace Stock */

#endif // STOCK_VIEW_HPP_INCLUDED
