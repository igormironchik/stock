
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

#ifndef STOCK_BY_SORT_FILTER_BASE_MODEL_HPP_INCLUDED
#define STOCK_BY_SORT_FILTER_BASE_MODEL_HPP_INCLUDED

// Qt include.
#include <QSortFilterProxyModel>
#include <QScopedPointer>


namespace Stock {

class SortFilterModel;

//
// SortFilterModelPrivate
//

class SortFilterModelPrivate {
public:
	explicit SortFilterModelPrivate( SortFilterModel * parent );
	virtual ~SortFilterModelPrivate();

	//! Code.
	QString m_code;
	//! Place.
	QString m_place;
	//! Description.
	QString m_desc;
	//! Parent.
	SortFilterModel * q;
}; // class SortFilterModelPrivate


//
// SortFilterModel
//

//! Sort-filter model for "By Product" mode.
class SortFilterModel
	:	public QSortFilterProxyModel
{
	Q_OBJECT

public:
	explicit SortFilterModel( QObject * parent );
	virtual ~SortFilterModel();

	//! Set filter data.
	void setFilterData( const QString & code, const QString & place,
		const QString & desc );

protected:
	SortFilterModel( SortFilterModelPrivate * dd, QObject * parent );

	QScopedPointer< SortFilterModelPrivate > d;

private:
	Q_DISABLE_COPY( SortFilterModel )
}; // class SortFilterModel

} /* namespace Stock */

#endif // STOCK_BY_SORT_FILTER_BASE_MODEL_HPP_INCLUDED
