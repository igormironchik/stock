
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_VIEW_HPP_INCLUDED
#define STOCK_VIEW_HPP_INCLUDED

// Qt include.
#include <QWidget>

// C++ include.
#include <memory>

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE


namespace Stock {

class ByProductSortModel;
class ByPlaceSortModel;
class ByProductView;
class ByPlaceView;
class ByProductModel;
class ByPlaceModel;
class DbSignals;
class Db;

//
// View
//

class ViewPrivate;

//! Central widget.
class View Q_DECL_FINAL
	:	public QWidget
{
	Q_OBJECT

public:
	explicit View( QWidget * parent );
	virtual ~View();

	//! \return By products view.
	ByProductView * byProductsView() const;
	//! \return By place view.
	ByPlaceView * byPlaceView() const;

	//! Set filter models.
	void setFilterModels( ByProductSortModel * product,
		ByPlaceSortModel * place );
	//! Set models.
	void setModels( ByProductModel * product,
		ByPlaceModel * place );
	//! Set DB.
	void setDb( DbSignals * sigs, Db * db );

public slots:
	//! Switch to by product code mode.
	void switchToByProductCode();
	//! Switch to by place mode.
	void switchToByPlace();

private slots:
	//! Filter changed.
	void filterChanged( const QString & );

private:
	friend class ViewPrivate;

	Q_DISABLE_COPY( View )

	std::unique_ptr< ViewPrivate > d;
}; // class View

} /* namespace Stock */

#endif // STOCK_VIEW_HPP_INCLUDED
