
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_BY_SORT_FILTER_BASE_MODEL_HPP_INCLUDED
#define STOCK_BY_SORT_FILTER_BASE_MODEL_HPP_INCLUDED

// Qt include.
#include <QSortFilterProxyModel>

// C++ include.
#include <memory>


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

	std::unique_ptr< SortFilterModelPrivate > d;

private:
	Q_DISABLE_COPY( SortFilterModel )
}; // class SortFilterModel

} /* namespace Stock */

#endif // STOCK_BY_SORT_FILTER_BASE_MODEL_HPP_INCLUDED
