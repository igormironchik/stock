
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
#include <QSqlError>


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

	QSqlQuery stockTable( QLatin1String(
		"CREATE TABLE IF NOT EXISTS stock ( code TEXT, place TEXT, "
		"amount INTEGER )" ) );

	stockTable.exec();

	QSqlQuery placeTable( QLatin1String( "CREATE TABLE places ( place TEXT )" ) );

	placeTable.exec();
}


//
// Db
//

Db::Db( QObject * parent )
	:	QObject( parent )
	,	d( new DbPrivate( this ) )
{
	d->init();
}

Db::~Db()
{
}

bool operator == ( const DbRecord & r1, const DbRecord & r2 )
{
	return ( r1.m_code == r2.m_code );
}

QVector< DbRecord >
Db::records( DbResult * res, QVector< DbRecord > * zeroProducts,
	QStringList * places ) const
{
	QVector< DbRecord > records;

	if( zeroProducts )
	{
		QSqlQuery all( "SELECT * FROM products" );

		if( res && all.lastError().isValid() )
		{
			*res = { false, all.lastError().text() };

			return records;
		}

		while( all.next() )
		{
			zeroProducts->push_back( { all.value( 0 ).toString(),
				QString(), 0, all.value( 1 ).toString() } );
		}
	}

	if( places )
	{
		QSqlQuery pl( "SELECT * FROM places" );

		if( res && pl.lastError().isValid() )
		{
			*res = { false, pl.lastError().text() };

			return records;
		}

		while( pl.next() )
			places->push_back( pl.value( 0 ).toString() );
	}

	QSqlQuery select( "SELECT t1.code, t1.place, t1.amount, t2.desc "
		"FROM stock AS t1, products AS t2 WHERE t1.code = t2.code" );

	if( res && select.lastError().isValid() )
	{
		*res = { false, select.lastError().text() };

		return records;
	}

	while( select.next() )
	{
		records.push_back( { select.value( 0 ).toString(),
			select.value( 1 ).toString(),
			select.value( 2 ).toULongLong(),
			select.value( 3 ).toString() } );

		if( zeroProducts )
			zeroProducts->removeOne( { select.value( 0 ).toString(),
				QString(), 0, QString() } );
	}

	if( res )
		*res = { true, QString() };

	return records;
}


//
// DbException
//

struct DbException {};

DbResult
Db::changeProduct( const DbRecord & r ) const
{
	if( !r.m_code.isEmpty() )
	{
		try {
			if( !d->m_connection.transaction() )
				return { false, d->m_connection.lastError().text() };

			QSqlQuery s1( "SELECT desc FROM products WHERE code = ?" );
			s1.addBindValue( r.m_code );

			if( !s1.exec() )
				throw DbException();

			if( s1.next() )
			{
				if( s1.value( 0 ).toString() != r.m_desc )
				{
					s1.finish();

					QSqlQuery update( "UPDATE products SET desc = ? WHERE code = ?" );
					update.addBindValue( r.m_desc );
					update.addBindValue( r.m_code );

					if( !update.exec() )
						throw DbException();
				}
			}
			else
			{
				s1.finish();

				QSqlQuery insert( "INSERT INTO products( code, desc ) VALUES( ?, ? )" );
				insert.addBindValue( r.m_code );
				insert.addBindValue( r.m_desc );

				if( !insert.exec() )
					throw DbException();
			}

			if( !r.m_place.isEmpty() )
			{
				QSqlQuery pc( "SELECT COUNT( * ) FROM places "
					"WHERE place = ?" );
				pc.addBindValue( r.m_place );

				if( !pc.exec() )
					throw DbException();

				pc.next();

				if( !pc.value( 0 ).toULongLong() )
				{
					QSqlQuery ip( "INSERT INTO places( place ) VALUES( ? )" );
					ip.addBindValue( r.m_place );

					if( !ip.exec() )
						throw DbException();
				}

				pc.finish();

				QSqlQuery s2( "SELECT COUNT( * ) FROM stock "
					"WHERE code = ? AND place = ?" );
				s2.addBindValue( r.m_code );
				s2.addBindValue( r.m_place );

				if( !s2.exec() )
					throw DbException();

				s2.next();

				if( s2.value( 0 ).toULongLong() > 0 )
				{
					s2.finish();

					if( r.m_count > 0 )
					{
						QSqlQuery update( "UPDATE stock SET amount = ? "
							"WHERE code = ? AND place = ?" );
						update.addBindValue( r.m_count );
						update.addBindValue( r.m_code );
						update.addBindValue( r.m_place );

						if( !update.exec() )
							throw DbException();
					}
					else
					{
						QSqlQuery del( "DELETE FROM stock "
							"WHERE code = ? AND place = ?" );
						del.addBindValue( r.m_code );
						del.addBindValue( r.m_place );

						if( !del.exec() )
							throw DbException();
					}
				}
				else if( r.m_count > 0 )
				{
					s2.finish();

					QSqlQuery insert( "INSERT INTO stock( code, place, amount ) "
						"VALUES( ?, ?, ? )" );
					insert.addBindValue( r.m_code );
					insert.addBindValue( r.m_place );
					insert.addBindValue( r.m_count );

					if( !insert.exec() )
						throw DbException();
				}
			}

			if( !d->m_connection.commit() )
				throw DbException();
		}
		catch( const DbException & )
		{
			d->m_connection.rollback();

			return { false, d->m_connection.lastError().text() };
		}
	}

	return { true, QString() };
}

DbResult
Db::deleteProduct( const QString & code )
{
	try {
		if( !d->m_connection.transaction() )
			return { false, d->m_connection.lastError().text() };

		QSqlQuery d1( "DELETE FROM products WHERE code = ?" );
		d1.addBindValue( code );

		if( !d1.exec() )
			throw DbException();

		QSqlQuery d2( "DELETE FROM stock WHERE code = ?" );
		d2.addBindValue( code );

		if( !d2.exec() )
			throw DbException();

		if( !d->m_connection.commit() )
			throw DbException();
	}
	catch( const DbException & )
	{
		d->m_connection.rollback();

		return { false, d->m_connection.lastError().text() };
	}

	return { true, QString() };
}

DbResult
Db::deletePlace( const QString & place )
{
	try {		
		QSqlQuery d1( "DELETE FROM places WHERE place = ?" );
		d1.addBindValue( place );

		if( !d1.exec() )
			throw DbException();
	}
	catch( const DbException & )
	{		
		return { false, d->m_connection.lastError().text() };
	}

	return { true, QString() };
}

DbResult
Db::changeCode( const QString & oldCode, const QString & newCode )
{
	try {
		if( !d->m_connection.transaction() )
			return { false, d->m_connection.lastError().text() };

		QSqlQuery u1( "UPDATE stock SET code = ? WHERE code = ?" );
		u1.addBindValue( newCode );
		u1.addBindValue( oldCode );

		if( !u1.exec() )
			throw DbException();

		QSqlQuery u2( "UPDATE products SET code = ? WHERE code = ?" );
		u2.addBindValue( newCode );
		u2.addBindValue( oldCode );

		if( !u2.exec() )
			throw DbException();

		if( !d->m_connection.commit() )
			throw DbException();
	}
	catch( const DbException & )
	{
		d->m_connection.rollback();

		return { false, d->m_connection.lastError().text() };
	}

	return { true, QString() };
}

DbResult
Db::renamePlace( const QString & oldPlace, const QString & newPlace )
{
	try {
		if( !d->m_connection.transaction() )
			return { false, d->m_connection.lastError().text() };

		QSqlQuery u1( "UPDATE stock SET place = ? WHERE place = ?" );
		u1.addBindValue( newPlace );
		u1.addBindValue( oldPlace );

		if( !u1.exec() )
			throw DbException();

		QSqlQuery u2( "UPDATE places SET place = ? WHERE place = ?" );
		u2.addBindValue( newPlace );
		u2.addBindValue( oldPlace );

		if( !u2.exec() )
			throw DbException();

		if( !d->m_connection.commit() )
			throw DbException();
	}
	catch( const DbException & )
	{
		d->m_connection.rollback();

		return { false, d->m_connection.lastError().text() };
	}

	return { true, QString() };
}

} /* namespace Stock */
