
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

#ifndef STOCK_DB_HPP_INCLUDED
#define STOCK_DB_HPP_INCLUDED

// Qt include.
#include <QScopedPointer>
#include <QVector>


namespace Stock {

//
// Record
//

//! Record in the database.
struct Record Q_DECL_FINAL {
	QString m_code;
	QString m_place;
	quint64 m_count;
	QString m_desc;
}; // struct Record


//
// Db
//

class DbPrivate;

class Db Q_DECL_FINAL
{
public:
	Db();
	~Db();

	//! \return All records.
	QVector< Record > records() const;
	//! Change product.
	void changeProduct( const Record & r ) const;
	//! Totally delete product. If you want to clear position on the place
	//! use changeProduct() with m_count = 0.
	void deleteProduct( const QString & code );

private:
	Q_DISABLE_COPY( Db )

	QScopedPointer< DbPrivate > d;
}; // class Db

} /* namespace Stock */

#endif // STOCK_DB_HPP_INCLUDED
