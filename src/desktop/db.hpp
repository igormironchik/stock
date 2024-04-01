
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_DB_HPP_INCLUDED
#define STOCK_DB_HPP_INCLUDED

// Qt include.
#include <QVector>
#include <QString>
#include <QObject>

// C++ include.
#include <memory>


namespace Stock {

//
// Record
//

//! Record in the database.
struct DbRecord Q_DECL_FINAL {
	QString m_code;
	QString m_place;
	quint64 m_count;
	QString m_desc;
}; // struct Record


//
// Result
//

//! Result of DB operation.
struct DbResult Q_DECL_FINAL {
	bool m_ok;
	QString m_error;
}; // struct Result


//
// Db
//

class DbPrivate;

class Db Q_DECL_FINAL
	:	public QObject
{
public:
	explicit Db( QObject * parent );
	virtual ~Db();

	//! \return All records.
	QVector< DbRecord > records( DbResult * res = Q_NULLPTR,
		QVector< DbRecord > * zeroProducts = Q_NULLPTR,
		QStringList * places = Q_NULLPTR ) const;
	//! Change product.
	DbResult changeProduct( const DbRecord & r ) const;
	//! Totally delete product. If you want to clear position on the place
	//! use changeProduct() with m_count = 0.
	DbResult deleteProduct( const QString & code );
	//! Delete place from places table. This will not affect stock table.
	DbResult deletePlace( const QString & place );
	//! Change code.
	DbResult changeCode( const QString & oldCode, const QString & newCode );
	//! Rename place.
	DbResult renamePlace( const QString & oldPlace, const QString & newPlace );

private:
	Q_DISABLE_COPY( Db )

	std::unique_ptr< DbPrivate > d;
}; // class Db

} /* namespace Stock */

#endif // STOCK_DB_HPP_INCLUDED
