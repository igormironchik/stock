
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_BY_PRODUCT_MODEL_HPP_INCLUDED
#define STOCK_BY_PRODUCT_MODEL_HPP_INCLUDED

// Qt include.
#include <QAbstractItemModel>

// C++ include.
#include <memory>

// Stock include.
#include "db.hpp"


namespace Stock {

class DbSignals;
class MainWindow;


//
// ByProductModel
//

class ByProductModelPrivate;

//! Model to show data in "By Product" mode.
class ByProductModel Q_DECL_FINAL
	:	public QAbstractItemModel
{
	Q_OBJECT

public:
	ByProductModel( MainWindow * win, Db * db,
		DbSignals * sigs, QObject * parent );
	virtual ~ByProductModel();

	//! \return List of all codes of products.
	QStringList codes() const;
	//! \return Description.
	QString desc( const QString & code ) const;
	//! \return Amount of product.
	quint64 count( const QString & code, const QString & place ) const;
	//! \return All records for the given product.
	QVector< DbRecord > records( const QString & code ) const;

	int columnCount( const QModelIndex & parent = QModelIndex() ) const
		Q_DECL_OVERRIDE;
	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const
		Q_DECL_OVERRIDE;
	Qt::ItemFlags flags( const QModelIndex & index ) const
		Q_DECL_OVERRIDE;
	bool hasChildren( const QModelIndex & parent = QModelIndex() ) const
		Q_DECL_OVERRIDE;
	QVariant headerData( int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole ) const Q_DECL_OVERRIDE;
	QModelIndex index( int row, int column,
		const QModelIndex & parent = QModelIndex() ) const
			Q_DECL_OVERRIDE;
	QModelIndex parent( const QModelIndex & index ) const
		Q_DECL_OVERRIDE;
	int rowCount( const QModelIndex & parent = QModelIndex() ) const
		Q_DECL_OVERRIDE;
	bool setData( const QModelIndex & index, const QVariant & value,
		int role = Qt::EditRole ) Q_DECL_OVERRIDE;

private slots:
	//! Product is deleted.
	void productDeleted(
		//! Code of the product. Can't be empty.
		const QString & code );
	//! Product's options are changed.
	void productChanged(
		//! Code of the product. Can't be empty.
		const QString & code,
		//! Place of the product. Can be empty, in this case
		//! product just was added into database.
		const QString & place,
		//! Amount of the product on the place. Ignores if product place is
		//! empty.
		quint64 count,
		//! Description of the product.
		const QString & desc );
	//! Code changed.
	void codeChanged( const QString & newCode, const QString & oldCode );
	//! Place renamed.
	void placeRenamed( const QString & newName, const QString & oldName );
	//! Place deleted.
	void placeDeleted( const QString & place );

private:
	friend class ByProductModelPrivate;

	Q_DISABLE_COPY( ByProductModel )

	std::unique_ptr< ByProductModelPrivate > d;
}; // class ByProductModel

} /* namespace Stock */

#endif // STOCK_BY_PRODUCT_MODEL_HPP_INCLUDED
