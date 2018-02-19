
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
#include "by_place_model.hpp"
#include "db.hpp"
#include "db_signals.hpp"
#include "mainwindow.hpp"

// Qt include.
#include <QVector>
#include <QSharedPointer>
#include <QMessageBox>
#include <QApplication>

 // C++ include.
#include <algorithm>


namespace Stock {

//
// IndexHelper
//

//! Internal index.
struct IndexHelper Q_DECL_FINAL {
	IndexHelper * m_parent;
	void * m_this;

	IndexHelper( IndexHelper * parent, void * self )
		:	m_parent( parent )
		,	m_this( self )
	{
	}
}; // struct IndexHelper


//
// ProductOnPlace
//

struct ProductOnPlace Q_DECL_FINAL {
	//! Code.
	QString m_code;
	//! Amount.
	quint64 m_count;
	//! Description.
	QString m_desc;
	//! Index.
	IndexHelper m_index;

	ProductOnPlace( const QString & code, quint64 count,
		const QString & desc, IndexHelper * parent )
		:	m_code( code )
		,	m_count( count )
		,	m_desc( desc )
		,	m_index( parent, this )
	{
	}
}; // struct ProductOnPlace


//
// Place
//

struct Place Q_DECL_FINAL {
	//! Place.
	QString m_place;
	//! Index.
	IndexHelper m_index;
	//! Products.
	QVector< QSharedPointer< ProductOnPlace > > m_prods;

	explicit Place( const QString & place )
		:	m_place( place )
		,	m_index( Q_NULLPTR, this )
	{
	}
}; // struct Place


//
// ByPlaceModelPrivate
//

class ByPlaceModelPrivate {
public:
	ByPlaceModelPrivate( MainWindow * win, Db * db,
		DbSignals * sigs, ByPlaceModel * parent )
		:	m_win( win )
		,	m_db( db )
		,	m_sigs( sigs )
		,	q( parent )
	{
	}

	//! Init.
	void init();
	//! \return Place object.
	Place * findPlace( const QString & place ) const;
	//! Add new place.
	void addNewPlace( const QString & place );
	//! Put new product on place.
	void putNewProductOnPlace( Place * pl, const QString & code,
		quint64 count, const QString & desc );
	//! \return Top row.
	int topRow( IndexHelper * index ) const;
	//! \return Total amount on place.
	quint64 totalCount( int index ) const;
	//! Change product.
	void changeProduct( Place * p, const QString & code, quint64 count,
		const QString & desc, bool add );

	//! Data.
	QVector< QSharedPointer< Place > > m_data;
	//! Main window.
	MainWindow * m_win;
	//! DB.
	Db * m_db;
	//! DB signals.
	DbSignals * m_sigs;
	//! Parent.
	ByPlaceModel * q;
}; // class ByPlaceModelPrivate

void
ByPlaceModelPrivate::init()
{
	QObject::connect( m_sigs, &DbSignals::productChanged,
		q, &ByPlaceModel::productChanged );
	QObject::connect( m_sigs, &DbSignals::productDeleted,
		q, &ByPlaceModel::productDeleted );

	DbResult state;

	const auto records = m_db->records( &state );

	if( !state.m_ok )
	{
		QMessageBox::critical( m_win,
			ByPlaceModel::tr( "Error in the database..." ),
			ByPlaceModel::tr( "Unable to read data from the database. "
				"Check database, please.\n\n%1" )
					.arg( state.m_error ) );

		QApplication::quit();
	}

	for( const auto & r : records )
	{
		auto * pl = findPlace( r.m_place );

		if( pl )
			putNewProductOnPlace( pl, r.m_code, r.m_count, r.m_desc );
		else
		{
			addNewPlace( r.m_place );
			putNewProductOnPlace( findPlace( r.m_place ), r.m_code,
				r.m_count, r.m_desc );
		}
	}

	if( !records.isEmpty() )
		emit q->dataChanged( q->index( 0, 0 ), q->index( q->rowCount() - 1, 1 ) );
}

bool operator == ( const QSharedPointer< Place > & p1,
	const QSharedPointer< Place > & p2 )
{
	return ( p1->m_place == p2->m_place );
}

Place *
ByPlaceModelPrivate::findPlace( const QString & place ) const
{
	const QSharedPointer< Place > p( new Place( place ) );

	auto it = std::find( m_data.begin(), m_data.end(), p );

	if( it != m_data.end() )
		return it->data();
	else
		return Q_NULLPTR;
}

void
ByPlaceModelPrivate::addNewPlace( const QString & place )
{
	const int row = m_data.size();

	q->beginInsertRows( QModelIndex(), row, row );
	m_data.push_back( QSharedPointer< Place > ( new Place( place ) ) );
	q->endInsertRows();
}

void
ByPlaceModelPrivate::putNewProductOnPlace( Place * pl, const QString & code,
	quint64 count, const QString & desc )
{
	if( pl )
	{
		const QModelIndex parent = q->createIndex(
			topRow( &pl->m_index ), 0, &pl->m_index );

		q->beginInsertRows( parent, pl->m_prods.size(), pl->m_prods.size() );
		pl->m_prods.push_back( QSharedPointer< ProductOnPlace > (
			new ProductOnPlace( code, count, desc, &pl->m_index ) ) );
		q->endInsertRows();
	}
}

int
ByPlaceModelPrivate::topRow( IndexHelper * index ) const
{
	int i = 0;

	for( const auto & p : qAsConst( m_data ) )
	{
		if( &p->m_index == index )
			return i;

		++i;
	}

	return -1;
}

quint64
ByPlaceModelPrivate::totalCount( int index ) const
{
	quint64 total = 0;

	for( const auto & p : qAsConst( m_data.at( index )->m_prods ) )
		total += p->m_count;

	return total;
}

void
ByPlaceModelPrivate::changeProduct( Place * p, const QString & code, quint64 count,
	const QString & desc, bool add )
{
	int i = 0;

	bool found = false;

	for( const auto & pr : qAsConst( p->m_prods ) )
	{
		if( pr->m_code == code )
		{
			found = true;

			if( add )
			{
				if( count == 0 )
				{
					const QModelIndex parent = q->createIndex( topRow( &p->m_index ),
						0, &p->m_index );

					q->beginRemoveRows( parent, i, i );
					p->m_prods.removeAt( i );
					q->endRemoveRows();

					break;
				}
				else
				{
					pr->m_count = count;
					pr->m_desc = desc;

					emit q->dataChanged( q->createIndex( i, 1, &pr->m_index ),
						q->createIndex( i, 2, &pr->m_index ) );

					break;
				}
			}
			else
			{
				if( pr->m_desc != desc )
				{
					pr->m_desc = desc;

					const QModelIndex index = q->createIndex( i, 2, &pr->m_index );

					emit q->dataChanged( index, index );

					break;
				}
			}
		}

		++i;
	}

	if( !found && add && count > 0 )
		putNewProductOnPlace( p, code, count, desc );
}


//
// ByPlaceModel
//

ByPlaceModel::ByPlaceModel( MainWindow * win, Db * db,
	DbSignals * sigs, QObject * parent )
	:	QAbstractItemModel( parent )
	,	d( new ByPlaceModelPrivate( win, db, sigs, this ) )
{
	d->init();
}

ByPlaceModel::~ByPlaceModel()
{
}

QStringList
ByPlaceModel::places() const
{
	QStringList res;

	for( const auto & p : qAsConst( d->m_data ) )
		res.push_back( p->m_place );

	return res;
}

int
ByPlaceModel::columnCount( const QModelIndex & ) const
{
	return 3;
}

QVariant
ByPlaceModel::data( const QModelIndex & index, int role ) const
{
	if( role != Qt::DisplayRole && role != Qt::EditRole )
		return QVariant();

	if( !index.parent().isValid() )
	{
		switch( index.column() )
		{
			case 0 :
				return d->m_data.at( index.row() )->m_place;

			case 1 :
				return d->totalCount( index.row() );

			default :
				return QVariant();
		}
	}
	else
	{
		switch( index.column() )
		{
			case 0 :
				return d->m_data.at( index.parent().row() )->
					m_prods.at( index.row() )->m_code;

			case 1 :
				return d->m_data.at( index.parent().row() )->
					m_prods.at( index.row() )->m_count;

			case 2 :
				return d->m_data.at( index.parent().row() )->
					m_prods.at( index.row() )->m_desc;

			default :
				return QVariant();
		}
	}
}

Qt::ItemFlags
ByPlaceModel::flags( const QModelIndex & index ) const
{
	if( !index.isValid() )
		return Qt::NoItemFlags;

	if( !index.parent().isValid() )
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
	{
		switch( index.column() )
		{
			case 0 :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

			case 1 :
			case 2 :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
					Qt::ItemIsEditable;

			default :
				return Qt::NoItemFlags;
		}
	}
}

bool
ByPlaceModel::hasChildren( const QModelIndex & parent ) const
{
	if( !parent.isValid() )
		return !d->m_data.isEmpty();

	if( !parent.parent().isValid() )
	{
		if( parent.row() >= 0 && parent.row() < d->m_data.size() )
			return ( !d->m_data.at( parent.row() )->m_prods.isEmpty() );
		else
			return false;
	}
	else
		return false;
}

QVariant
ByPlaceModel::headerData( int section, Qt::Orientation orientation,
	int role ) const
{
	if( role != Qt::DisplayRole )
		return QVariant();

	if( orientation == Qt::Horizontal )
	{
		switch ( section )
		{
			case 0 : return tr( "Place / Product Code" );
			case 1 : return tr( "Amount" );
			case 2 : return tr( "Description" );

			default : return QVariant();
		}
	}

	return QVariant();
}

QModelIndex
ByPlaceModel::index( int row, int column,
	const QModelIndex & parent ) const
{
	if( !parent.isValid() )
	{
		if( row >= 0 && row < d->m_data.size() )
			return createIndex( row, column, &d->m_data.at( row )->m_index );
		else
			return QModelIndex();
	}
	else
	{
		auto * p = static_cast< Place* > ( static_cast< IndexHelper* > (
			parent.internalPointer() )->m_this );

		if( p && row >= 0 && row < p->m_prods.size() )
			return createIndex( row, column,
				&p->m_prods.at( row )->m_index );
		else
			return QModelIndex();
	}
}

QModelIndex
ByPlaceModel::parent( const QModelIndex & index ) const
{
	if( index.isValid() )
	{
		IndexHelper * helper = static_cast< IndexHelper* > (
			index.internalPointer() );

		if( !helper->m_parent )
			return QModelIndex();
		else
			return createIndex( d->topRow( helper->m_parent ),
				0, helper->m_parent );
	}
	else
		return QModelIndex();
}

int
ByPlaceModel::rowCount( const QModelIndex & parent ) const
{
	if( parent.isValid() )
	{
		const auto * helper = static_cast< IndexHelper* > (
			parent.internalPointer() );

		return ( static_cast< Place* > ( helper->m_this )->m_prods.size() );
	}
	else
		return d->m_data.size();
}

bool
ByPlaceModel::setData( const QModelIndex & index, const QVariant & value,
	int role )
{
	if( role != Qt::EditRole )
		return false;

	if( index.parent().isValid() )
	{
		switch( index.column() )
		{
			case 1 :
			{
				auto * place = static_cast< Place* > (
					static_cast< IndexHelper* > (
						index.internalPointer() )->m_parent->m_this );

				auto * product = static_cast< ProductOnPlace* > (
					static_cast< IndexHelper* > (
						index.internalPointer() )->m_this );

				DbResult result = d->m_db->changeProduct( { product->m_code,
					place->m_place, value.toULongLong(),
						product->m_desc } );

				if( result.m_ok )
				{
					d->m_sigs->emitProductChanged( product->m_code,
						place->m_place, value.toULongLong(), product->m_desc );

					return true;
				}
				else
				{
					QMessageBox::critical( d->m_win,
						tr( "Error in the database..." ),
						tr( "Unable to change amount of the product.\n\n%1" )
							.arg( result.m_error ) );

					return false;
				}
			}

			case 2 :
			{
				auto * product = static_cast< ProductOnPlace* > (
					static_cast< IndexHelper* > (
						index.internalPointer() )->m_this );

				DbResult result = d->m_db->changeProduct(
					{ product->m_code, QString(), 0,
						value.toString() } );

				if( result.m_ok )
				{
					d->m_sigs->emitProductChanged( product->m_code, QString(), 0,
						value.toString() );

					return true;
				}
				else
				{
					QMessageBox::critical( d->m_win, tr( "Error in the database..." ),
						tr( "Unable to change description of the product.\n\n%1" )
							.arg( result.m_error ) );

					return false;
				}
			}

			default :
				return false;
		}
	}
	else
		return false;
}

void
ByPlaceModel::productDeleted( const QString & code )
{
	for( auto i = 0; i < d->m_data.size(); ++i )
	{
		Place * place = d->m_data.at( i ).data();

		for( auto j = 0; j < place->m_prods.size(); ++j )
		{
			if( place->m_prods.at( j )->m_code == code )
			{
				beginRemoveRows( createIndex( i, 0, &place->m_index ), j, j );
				place->m_prods.removeAt( j );
				endRemoveRows();
				break;
			}
		}
	}
}

void
ByPlaceModel::productChanged( const QString & code, const QString & place,
	quint64 count, const QString & desc )
{
	if( !place.isEmpty() )
	{
		Place * p = d->findPlace( place );

		if( p )
			d->changeProduct( p, code, count, desc, true );
		else
		{
			d->addNewPlace( place );

			p = d->findPlace( place );

			d->changeProduct( p, code, count, desc, true );
		}
	}
	else
	{
		for( const auto & p : qAsConst( d->m_data ) )
			d->changeProduct( p.data(), code, count, desc, false );
	}
}

} /* namespace Stock */
