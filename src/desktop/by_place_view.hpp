
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_BY_PLACE_VIEW_HPP_INCLUDED
#define STOCK_BY_PLACE_VIEW_HPP_INCLUDED

// Qt include.
#include <QTreeView>

// C++ include.
#include <memory>


namespace Stock {

class ByPlaceModel;
class ByPlaceSortModel;
class DbSignals;
class Db;
class ByProductModel;

//
// ByPlaceView
//

class ByPlaceViewPrivate;

//! View for "By Product" mode.
class ByPlaceView Q_DECL_FINAL
	:	public QTreeView
{
	Q_OBJECT

signals:
	//! Update size hint for description column.
	void updateDescColumn( int section, int, int );

public:
	explicit ByPlaceView( QWidget * parent );
	virtual ~ByPlaceView();

	//! Set filter model.
	void setFilterModel( ByPlaceSortModel * filter );
	//! Set source model.
	void setSourceModel( ByPlaceModel * model );
	//! Set auxiliary model.
	void setAuxiliaryModel( ByProductModel * model );
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
	//! Delete place.
	void deletePlace();
	//! Change amount of product.
	void changeAmount();

private:
	friend class ByPlaceViewPrivate;

	Q_DISABLE_COPY( ByPlaceView )

	std::unique_ptr< ByPlaceViewPrivate > d;
}; // class ByPlaceView

} /* namespace Stock */

#endif // STOCK_BY_PLACE_VIEW_HPP_INCLUDED
