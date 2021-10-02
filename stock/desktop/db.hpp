
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2018-2020 Igor Mironchik

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
