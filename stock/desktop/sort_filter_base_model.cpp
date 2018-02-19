
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
