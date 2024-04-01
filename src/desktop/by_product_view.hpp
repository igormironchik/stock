
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_BY_PRODUCT_VIEW_HPP_INCLUDED
#define STOCK_BY_PRODUCT_VIEW_HPP_INCLUDED

// Qt include.
#include <QTreeView>

// C++ include.
#include <memory>


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

	std::unique_ptr< ByProductViewPrivate > d;
}; // class ByProductView

} /* namespace Stock */

#endif // STOCK_BY_PRODUCT_VIEW_HPP_INCLUDED
