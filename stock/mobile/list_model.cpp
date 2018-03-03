
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
#include "list_model.hpp"

// Qt include.
#include <QVector>


namespace Stock {

//
// Data
//

//! Row in the model.
struct Data {
	QString m_code;
	QString m_place;
	quint64 m_count;
	QString m_desc;
}; // struct Data


//
// ListModelPrivate
//

class ListModelPrivate {
public:
	ListModelPrivate( ListModel * parent )
		:	q( parent )
	{
	}

	//! Data.
	QVector< Data > m_data;
	//! Parent.
	ListModel * q;
}; // class ListModelPrivate


//
// ListModel
//

ListModel::ListModel( QObject * parent )
	:	QAbstractListModel( parent )
	,	d( new ListModelPrivate( this ) )
{
}

ListModel::~ListModel()
{
}

void
ListModel::setData( const std::vector< Messages::Product > & data )
{
	beginResetModel();
	d->m_data.clear();

	for( const auto & p : data )
		d->m_data.push_back( { p.code(), p.place(), p.count(), p.desc() } );

	endResetModel();
}

int
ListModel::rowCount( const QModelIndex & parent ) const
{
	if( !parent.isValid() )
		return d->m_data.size();
	else
		return 0;
}

QVariant
ListModel::data( const QModelIndex & index, int role ) const
{
	switch( role )
	{
		case CodeRole :
			return d->m_data.at( index.row() ).m_code;

		case PlaceRole :
			return d->m_data.at( index.row() ).m_place;

		case AmountRole :
			return d->m_data.at( index.row() ).m_count;

		case DescRole :
			return d->m_data.at( index.row() ).m_desc;

		default :
			return QVariant();
	}
}

QHash< int, QByteArray >
ListModel::roleNames() const
{
	QHash< int, QByteArray > names;

	names[ CodeRole ] = "CodeRole";
	names[ DescRole ] = "DescRole";
	names[ PlaceRole ] = "PlaceRole";
	names[ AmountRole ] = "AmountRole";

	return names;
}

} /* namespace Stock */
