
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

// Stock include.
#include "db.hpp"
#include "shared/exceptions.hpp"

// Qt include.
#include <QSqlDatabase>
#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>


namespace Stock {

//
// DbPrivate
//

class DbPrivate {
public:
	DbPrivate( Db * parent )
		:	q( parent )
	{
	}

	~DbPrivate()
	{
		m_connection.close();
	}

	//! Init.
	void init();

	//! Connection.
	QSqlDatabase m_connection;
	//! Parent.
	Db * q;
}; // class DbPrivate

static const QString c_dbFileName = QLatin1String( "./stock.db" );

void
DbPrivate::init()
{
	m_connection = QSqlDatabase::addDatabase( "QSQLITE" );
	m_connection.setDatabaseName( c_dbFileName );

	if( !m_connection.open() )
		throw Exception( QObject::tr( "Unable to open database: %1" )
			.arg( m_connection.lastError().text() ) );

	QSqlQuery productsTable( QLatin1String(
		"CREATE TABLE IF NOT EXISTS products ( code TEXT, desc TEXT )" ) );

	productsTable.exec();

	QSqlQuery placeTable( QLatin1String(
		"CREATE TABLE IF NOT EXISTS places ( code TEXT, place TEXT, "
		"amount INTEGER )" ) );

	placeTable.exec();
}


//
// Db
//

Db::Db()
	:	d( new DbPrivate( this ) )
{
	d->init();
}

Db::~Db()
{
}

QVector< Record >
Db::records() const
{
	QVector< Record > res;

	QSqlQuery select( "SELECT t1.code, t1.place, t1.amount, t2.desc "
		"FROM places AS t1, products AS t2 WHERE t1.code = t2.code" );

	while( select.next() )
	{
		res.push_back( { select.value( 0 ).toString(),
			select.value( 1 ).toString(),
			select.value( 2 ).toULongLong(),
			select.value( 3 ).toString() } );
	}

	return res;
}

void
Db::changeProduct( const Record & r ) const
{
	if( !r.m_code.isEmpty() )
	{
		QSqlQuery s1( "SELECT desc FROM products WHERE code = ?" );
		s1.addBindValue( r.m_code );
		s1.exec();

		if( s1.next() )
		{
			if( s1.value( 0 ).toString() != r.m_desc )
			{
				QSqlQuery update( "UPDATE products SET desc = ? WHERE code = ?" );
				update.addBindValue( r.m_desc );
				update.addBindValue( r.m_code );
				update.exec();
			}
		}
		else
		{
			QSqlQuery insert( "INSERT INTO products( code, desc ) VALUES( ?, ? )" );
			insert.addBindValue( r.m_code );
			insert.addBindValue( r.m_desc );
			insert.exec();
		}

		if( !r.m_place.isEmpty() )
		{
			QSqlQuery s2( "SELECT COUNT( * ) FROM places "
				"WHERE code = ? AND place = ?" );
			s2.addBindValue( r.m_code );
			s2.addBindValue( r.m_place );
			s2.exec();
			s2.next();

			if( s2.value( 0 ).toULongLong() > 0 )
			{
				if( r.m_count > 0 )
				{
					QSqlQuery update( "UPDATE places SET amount = ? "
						"WHERE code = ? AND place = ?" );
					update.addBindValue( r.m_count );
					update.addBindValue( r.m_code );
					update.addBindValue( r.m_place );
					update.exec();
				}
				else
				{
					QSqlQuery del( "DELETE FROM places "
						"WHERE code = ? AND place = ?" );
					del.addBindValue( r.m_code );
					del.addBindValue( r.m_place );
					del.exec();
				}
			}
			else if( r.m_count > 0 )
			{
				QSqlQuery insert( "INSERT INTO places( code, place, amount ) "
					"VALUES( ?, ?, ? )" );
				insert.addBindValue( r.m_code );
				insert.addBindValue( r.m_place );
				insert.addBindValue( r.m_count );
				insert.exec();
			}
		}
	}
}

void
Db::deleteProduct( const QString & code )
{
	QSqlQuery d1( "DELETE FROM products WHERE code = ?" );
	d1.addBindValue( code );
	d1.exec();

	QSqlQuery d2( "DELETE FROM places WHERE code = ?" );
	d2.addBindValue( code );
	d2.exec();
}

} /* namespace Stock */
