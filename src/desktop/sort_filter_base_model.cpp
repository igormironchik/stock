
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "sort_filter_base_model.hpp"


namespace Stock {

//
// SortFilterModelPrivate
//

SortFilterModelPrivate::SortFilterModelPrivate( SortFilterModel * parent )
	:	q( parent )
{
}

SortFilterModelPrivate::~SortFilterModelPrivate()
{
}


//
// SortFilterModel
//

SortFilterModel::SortFilterModel( QObject * parent )
	:	QSortFilterProxyModel( parent )
	,	d( new SortFilterModelPrivate( this ) )
{
}

SortFilterModel::SortFilterModel( SortFilterModelPrivate * dd, QObject * parent )
	:	QSortFilterProxyModel( parent )
	,	d( dd )
{
}

SortFilterModel::~SortFilterModel()
{
}

void
SortFilterModel::setFilterData( const QString & code, const QString & place,
	const QString & desc )
{
	d->m_code = code;
	d->m_place = place;
	d->m_desc = desc;

	invalidateFilter();
}

} /* namespace Stock */
