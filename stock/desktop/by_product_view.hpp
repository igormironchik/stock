
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

#ifndef STOCK_BY_PRODUCT_VIEW_HPP_INCLUDED
#define STOCK_BY_PRODUCT_VIEW_HPP_INCLUDED

// Qt include.
#include <QTreeView>
#include <QScopedPointer>


namespace Stock {

class ByProductModel;
class ByProductSortModel;
class DbSignals;
class Db;
class ByPlaceModel;

//
// ByProductView
//

class ByProductViewPrivate;

//! View for "By Product" mode.
class ByProductView Q_DECL_FINAL
	:	public QTreeView
{
	Q_OBJECT

signals:
	//! Update size hint for description column.
	void updateDescColumn( int section, int, int );

public:
	explicit ByProductView( QWidget * parent );
	virtual ~ByProductView();

	//! Set filter model.
	void setFilterModel( ByProductSortModel * filter );
	//! Set source model.
	void setSourceModel( ByProductModel * model );
	//! Set auxiliary model.
	void setAuxiliaryModel( ByPlaceModel * model );
	//! Set DB.
	void setDb( DbSignals * sigs, Db * db );

protected:
	void contextMenuEvent( QContextMenuEvent * e ) Q_DECL_OVERRIDE;

public slots:
	//! Section resized.
	void sectionResized( int section, int, int );

private slots:
	//! Change code.
	void changeCode();
	//! Change description.
	void changeDesc();
	//! Rename place.
	void renamePlace();
	//! Delete product.
	void deleteProduct();
	//! Change amount of product.
	void changeAmount();

private:
	friend class ByProductViewPrivate;

	Q_DISABLE_COPY( ByProductView )

	QScopedPointer< ByProductViewPrivate > d;
}; // class ByProductView

} /* namespace Stock */

#endif // STOCK_BY_PRODUCT_VIEW_HPP_INCLUDED
