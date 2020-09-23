
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

#ifndef STOCK_LIST_MODEL_HPP_INCLUDED
#define STOCK_LIST_MODEL_HPP_INCLUDED

// Qt include.
#include <QAbstractListModel>
#include <QScopedPointer>

// Stock include.
#include "messages.hpp"


namespace Stock {

//
// ListModel
//

class ListModelPrivate;

//! Model for the search result.
class ListModel Q_DECL_FINAL
	:	public QAbstractListModel
{
	Q_OBJECT

public:
	explicit ListModel( QObject * parent );
	virtual ~ListModel();

	//! Set data.
	void setData( const std::vector< Messages::Product > & data );

	enum RoleNames {
		CodeRole = Qt::UserRole,
		DescRole = CodeRole + 1,
		PlaceRole = DescRole + 1,
		AmountRole = PlaceRole + 1,
	}; // enum RoleNames

	int rowCount( const QModelIndex & parent = QModelIndex() ) const
		Q_DECL_OVERRIDE;
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const
		Q_DECL_OVERRIDE;
	QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY( ListModel )

	QScopedPointer< ListModelPrivate > d;
}; // class ListModel

} /* namespace Stock */

#endif // STOCK_LIST_MODEL_HPP_INCLUDED
