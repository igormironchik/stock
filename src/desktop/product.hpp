
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_PRODUCT_HPP_INCLUDED
#define STOCK_PRODUCT_HPP_INCLUDED

// Qt include.
#include <QDialog>

// C++ include.
#include <memory>


namespace Stock {

class ByProductModel;


//
// ProductDlg
//

class ProductDlgPrivate;

//! Dialog with settings of product.
class ProductDlg Q_DECL_FINAL
	:	public QDialog
{
	Q_OBJECT

public:
	ProductDlg( const QString & code,
		const QString & place,
		quint64 count,
		const QString & desc,
		const QStringList & codes,
		const QStringList & places,
		bool isNewProduct,
		ByProductModel * model,
		QWidget * parent );
	virtual ~ProductDlg();

	//! \return Code.
	QString code() const;
	//! \return Place.
	QString place() const;
	//! \return Amount.
	quint64 count() const;
	//! \return Description.
	QString desc() const;

private slots:
	//! Code changed.
	void codeChanged( const QString & code );
	//! Place changed.
	void placeChanged( const QString & place );

private:
	Q_DISABLE_COPY( ProductDlg )

	std::unique_ptr< ProductDlgPrivate > d;
}; // class ProductDlg

} /* namespace Stock */

#endif // STOCK_PRODUCT_HPP_INCLUDED
