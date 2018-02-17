
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

#ifndef STOCK_BY_PRODUCT_MODEL_HPP_INCLUDED
#define STOCK_BY_PRODUCT_MODEL_HPP_INCLUDED

// Qt include.
#include <QAbstractItemModel>
#include <QScopedPointer>


namespace Stock {

class Db;
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
	//! \return List of all places.
	QStringList places() const;

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

private:
	friend class ByProductModelPrivate;

	Q_DISABLE_COPY( ByProductModel )

	QScopedPointer< ByProductModelPrivate > d;
}; // class ByProductModel

} /* namespace Stock */

#endif // STOCK_BY_PRODUCT_MODEL_HPP_INCLUDED
