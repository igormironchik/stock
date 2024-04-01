
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_LIST_MODEL_HPP_INCLUDED
#define STOCK_LIST_MODEL_HPP_INCLUDED

// Qt include.
#include <QAbstractListModel>

// C++ include.
#include <memory>

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
	void setVectorData( const std::vector< Messages::Product > & data );

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

	std::unique_ptr< ListModelPrivate > d;
}; // class ListModel

} /* namespace Stock */

#endif // STOCK_LIST_MODEL_HPP_INCLUDED
