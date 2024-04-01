
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_DB_SIGNALS_HPP_INCLUDED
#define STOCK_DB_SIGNALS_HPP_INCLUDED

// Qt include.
#include <QObject>


namespace Stock {

//
// DbSignals
//

//! Signals to inform database.
//! This object is used for synchronization of models and database object.
class DbSignals Q_DECL_FINAL
	:	public QObject
{
	Q_OBJECT

signals:
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
	//! Place deleted (only place, stock database is untouched).
	void placeDeleted( const QString & place );
	//! Product's code changed.
	void codeChanged( const QString & newCode, const QString & oldCode );
	//! Place renamed.
	void placeRenamed( const QString & newName, const QString & oldName );

public:
	explicit DbSignals( QObject * parent );
	virtual ~DbSignals();

public slots:
	//! Product is deleted.
	void emitProductDeleted(
		//! Code of the product. Can't be empty.
		const QString & code );
	//! Product's options are changed.
	void emitProductChanged(
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
	//! Product's code changed.
	void emitCodeChanged( const QString & newCode, const QString & oldCode );
	//! Place renamed.
	void emitPlaceRenamed( const QString & newName, const QString & oldName );
	//! Place deleted (only place, stock database is untouched).
	void emitPlaceDeleted( const QString & place );
}; // class DbSignals

} /* namespace Stock */

#endif // STOCK_DB_SIGNALS_HPP_INCLUDED
