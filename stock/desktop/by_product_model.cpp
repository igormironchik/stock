
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
#include "by_product_model.hpp"
#include "db.hpp"
#include "db_signals.hpp"
#include "mainwindow.hpp"

// Qt include.
#include <QVector>
#include <QMessageBox>
#include <QApplication>
#include <QSharedPointer>

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

//! Product data on the give place.
struct ProductOnPlace Q_DECL_FINAL {
	//! Place.
	QString m_place;
	//! Amount of the product.
	quint64 m_count;
	//! Internal index.
	IndexHelper m_index;

	ProductOnPlace( const QString & place, quint64 count, IndexHelper * parent )
		:	m_place( place )
		,	m_count( count )
		,	m_index( parent, this )
	{
	}
}; // struct ProductOnPlace


//
// Product
//

//! Product.
struct Product Q_DECL_FINAL {
	//! Code.
	QString m_code;
	//! Description.
	QString m_desc;
	//! Product on places.
	QVector< QSharedPointer< ProductOnPlace > > m_places;
	//! Internal index.
	IndexHelper m_index;

	Product( const QString & code, const QString & desc )
		:	m_code( code )
		,	m_desc( desc )
		,	m_index( Q_NULLPTR, this )
	{
	}
}; // struct Product


//
// ByProductModelPrivate
//

class ByProductModelPrivate {
public:
	ByProductModelPrivate( MainWindow * win, Db * db, DbSignals * sigs,
		ByProductModel * parent )
		:	m_win( win )
		,	m_db( db )
		,	m_sigs( sigs )
		,	q( parent )
	{
	}

	//! Init.
	void init();
	//! \return Product by the given code.
	Product * findProduct( const QString & code );
	//! \return Total amount of product.
	quint64 totalCount( int index ) const;
	//! \return Top row.
	int topRow( IndexHelper * index ) const;
	//! \return Place.
	ProductOnPlace * findPlace( Product * p, const QString & place );
	//! \return Row of the place.
	int placeRow( Product * p, const QString & place ) const;
	//! Add new product.
	void addNewProduct( const QString & code, const QString & desc );
	//! Put new product on place.
	void putNewProductOnPlace( Product * product, const QString & place,
		quint64 count );

	//! Data.
	QVector< QSharedPointer< Product > > m_data;
	//! Main window.
	MainWindow * m_win;
	//! DB.
	Db * m_db;
	//! DB signals.
	DbSignals * m_sigs;
	//! Parent.
	ByProductModel * q;
}; // class ByProductModelPrivate

void
ByProductModelPrivate::init()
{
	QObject::connect( m_sigs, &DbSignals::productChanged,
		q, &ByProductModel::productChanged );
	QObject::connect( m_sigs, &DbSignals::productDeleted,
		q, &ByProductModel::productDeleted );

	DbResult state;

	const auto records = m_db->records( &state );

	if( !state.m_ok )
	{
		QMessageBox::critical( m_win,
			ByProductModel::tr( "Error in the database..." ),
			ByProductModel::tr( "Unable to read data from the database. "
				"Check database, please.\n\n%1" )
					.arg( state.m_error ) );

		QApplication::quit();
	}

	for( const auto & r : records )
	{
		auto * pr = findProduct( r.m_code );

		if( pr )
			putNewProductOnPlace( pr, r.m_place, r.m_count );
		else
		{
			addNewProduct( r.m_code, r.m_desc );
			putNewProductOnPlace( findProduct( r.m_code ), r.m_place, r.m_count );
		}
	}

	if( !records.isEmpty() )
		emit q->dataChanged( q->index( 0, 0 ), q->index( q->rowCount() - 1, 2 ) );
}

bool operator == ( const QSharedPointer< Product > & p1,
	const QSharedPointer< Product > & p2 )
{
	return ( p1->m_code == p2->m_code );
}

Product *
ByProductModelPrivate::findProduct( const QString & code )
{
	const QSharedPointer< Product > p( new Product( code, "" ) );

	auto it = std::find( m_data.begin(), m_data.end(), p );

	if( it != m_data.end() )
		return it->data();
	else
		return Q_NULLPTR;
}

quint64
ByProductModelPrivate::totalCount( int index ) const
{
	quint64 total = 0;

	for( const auto & p : qAsConst( m_data.at( index )->m_places ) )
		total += p->m_count;

	return total;
}

int
ByProductModelPrivate::topRow( IndexHelper * index ) const
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

bool operator == ( const QSharedPointer< ProductOnPlace > & p1,
	const QSharedPointer< ProductOnPlace > & p2 )
{
	return ( p1->m_place == p2->m_place );
}

ProductOnPlace *
ByProductModelPrivate::findPlace( Product * p, const QString & place )
{
	const QSharedPointer< ProductOnPlace > tmp(
		new ProductOnPlace( place, 0, Q_NULLPTR ) );

	auto it = std::find( p->m_places.begin(), p->m_places.end(), tmp );

	if( it != p->m_places.end() )
		return it->data();
	else
		return Q_NULLPTR;
}

int
ByProductModelPrivate::placeRow( Product * p, const QString & place ) const
{
	int i = 0;

	for( const auto & onPlace : qAsConst( p->m_places ) )
	{
		if( onPlace->m_place == place )
			return i;

		++i;
	}

	return -1;
}

void
ByProductModelPrivate::addNewProduct( const QString & code,
	const QString & desc )
{
	const int row = m_data.size();

	q->beginInsertRows( QModelIndex(), row, row );
	m_data.push_back( QSharedPointer< Product > (
		new Product( code, desc ) ) );
	q->endInsertRows();
}

void
ByProductModelPrivate::putNewProductOnPlace( Product * product,
	const QString & place, quint64 count )
{
	if( product )
	{
		const QModelIndex parent = q->createIndex(
			topRow( &product->m_index ), 0, &product->m_index );

		q->beginInsertRows( parent, product->m_places.size(),
			product->m_places.size() );
		product->m_places.push_back( QSharedPointer< ProductOnPlace > (
			new ProductOnPlace( place, count, &product->m_index ) ) );
		q->endInsertRows();
	}
}


//
// ByProductModel
//

ByProductModel::ByProductModel( MainWindow * win, Db * db, DbSignals * sigs,
	QObject * parent )
	:	QAbstractItemModel( parent )
	,	d( new ByProductModelPrivate( win, db, sigs, this ) )
{
	d->init();
}

ByProductModel::~ByProductModel()
{
}

QStringList
ByProductModel::codes() const
{
	QStringList res;

	for( const auto & p : qAsConst( d->m_data ) )
		res.append( p->m_code );

	return res;
}

QStringList
ByProductModel::places() const
{
	QStringList res;

	for( const auto & p : qAsConst( d->m_data ) )
		for( const auto & place : qAsConst( p->m_places ) )
			if( !res.contains( place->m_place ) )
				res.append( place->m_place );

	return res;
}

QString
ByProductModel::desc( const QString & code ) const
{
	const auto * p = d->findProduct( code );

	if( p )
		return p->m_desc;
	else
		return QString();
}

quint64
ByProductModel::count( const QString & code, const QString & place ) const
{
	auto * p = d->findProduct( code );

	if( p )
	{
		const auto * onPlace = d->findPlace( p, place );

		if( onPlace )
			return onPlace->m_count;
	}

	return 0;
}

int
ByProductModel::columnCount( const QModelIndex & ) const
{
	return 3;
}

QVariant
ByProductModel::data( const QModelIndex & index, int role ) const
{
	if( role != Qt::DisplayRole && role != Qt::EditRole )
		return QVariant();

	if( !index.parent().isValid() )
	{
		switch( index.column() )
		{
			case 0 :
				return d->m_data.at( index.row() )->m_code;

			case 1 :
				return d->totalCount( index.row() );

			case 2 :
				return d->m_data.at( index.row() )->m_desc;

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
					m_places.at( index.row() )->m_place;

			case 1 :
				return d->m_data.at( index.parent().row() )->
					m_places.at( index.row() )->m_count;

			default :
				return QVariant();
		}
	}
}

Qt::ItemFlags
ByProductModel::flags( const QModelIndex & index ) const
{
	if( !index.isValid() )
		return Qt::NoItemFlags;

	if( !index.parent().isValid() )
	{
		switch( index.column() )
		{
			case 0 :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

			case 1 :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

			case 2 :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
					Qt::ItemIsEditable;

			default :
				return Qt::NoItemFlags;
		}
	}
	else
	{
		switch( index.column() )
		{
			case 0 :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

			case 1 :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
					Qt::ItemIsEditable;

			default :
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}
	}
}

bool
ByProductModel::hasChildren( const QModelIndex & parent ) const
{
	if( !parent.isValid() )
		return !d->m_data.isEmpty();

	if( !parent.parent().isValid() )
	{
		if( parent.row() >= 0 && parent.row() < d->m_data.size() )
			return ( !d->m_data.at( parent.row() )->m_places.isEmpty() );
		else
			return false;
	}
	else
		return false;
}

QVariant
ByProductModel::headerData( int section, Qt::Orientation orientation,
	int role ) const
{
	if( role != Qt::DisplayRole )
		return QVariant();

	if( orientation == Qt::Horizontal )
	{
		switch ( section )
		{
			case 0 : return tr( "Product Code / Place" );
			case 1 : return tr( "Amount" );
			case 2 : return tr( "Description" );

			default : return QVariant();
		}
	}

	return QVariant();
}

QModelIndex
ByProductModel::index( int row, int column, const QModelIndex & parent ) const
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
		auto * p = static_cast< Product* > ( static_cast< IndexHelper* > (
			parent.internalPointer() )->m_this );

		if( p && row >= 0 && row < p->m_places.size() )
			return createIndex( row, column,
				&p->m_places.at( row )->m_index );
		else
			return QModelIndex();
	}
}

QModelIndex
ByProductModel::parent( const QModelIndex & index ) const
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
ByProductModel::rowCount( const QModelIndex & parent ) const
{
	if( parent.isValid() )
	{
		const auto * helper = static_cast< IndexHelper* > (
			parent.internalPointer() );

		return ( static_cast< Product* > ( helper->m_this )->m_places.size() );
	}
	else
		return d->m_data.size();
}

bool
ByProductModel::setData( const QModelIndex & index, const QVariant & value,
	int role )
{
	if( role != Qt::EditRole )
		return false;

	if( !index.parent().isValid() )
	{
		if( index.column() == 2 )
		{
			auto * p = d->m_data.at( index.row() ).data();

			DbResult result = d->m_db->changeProduct( { p->m_code, QString(), 0,
				value.toString() } );

			if( result.m_ok )
			{
				d->m_sigs->emitProductChanged( p->m_code, QString(), 0,
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
		else
			return false;
	}
	else
	{
		const qint64 tmpValue = value.toLongLong();

		if( tmpValue >= 0 )
		{
			if( index.column() == 1 )
			{
				auto * p = static_cast< Product* > (
					static_cast< IndexHelper* > (
						index.internalPointer() )->m_parent->m_this );

				DbResult result = d->m_db->changeProduct( { p->m_code,
					p->m_places.at( index.row() )->m_place, value.toULongLong(),
						p->m_desc } );

				if( result.m_ok )
				{
					d->m_sigs->emitProductChanged( p->m_code,
						p->m_places.at( index.row() )->m_place,
						value.toULongLong(), p->m_desc );

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
			else
				return false;
		}
		else
			return false;
	}
}

void
ByProductModel::productDeleted( const QString & code )
{
	Product * p = d->findProduct( code );

	if( p )
	{
		const int row = d->topRow( &p->m_index );

		if( row >= 0 )
		{
			beginRemoveRows( QModelIndex(), row, row );
			d->m_data.removeAt( row );
			endRemoveRows();
		}
	}
}

void
ByProductModel::productChanged( const QString & code, const QString & place,
	quint64 count, const QString & desc )
{
	auto * p = d->findProduct( code );

	if( p )
	{
		if( p->m_desc != desc )
		{
			p->m_desc = desc;

			const QModelIndex index = createIndex( d->topRow( &p->m_index ), 2,
				&p->m_index );

			emit dataChanged( index, index );
		}

		if( !place.isEmpty() )
		{
			ProductOnPlace * onPlace = d->findPlace( p, place );

			if( onPlace )
			{
				if( onPlace->m_count != count )
				{
					if( count > 0 )
					{
						onPlace->m_count = count;

						const QModelIndex index = createIndex(
							d->placeRow( p, place ), 1, &onPlace->m_index );

						emit dataChanged( index, index );
					}
					else
					{
						const QModelIndex parent = createIndex(
							d->topRow( &p->m_index ), 0, &p->m_index );
						const int row = d->placeRow( p, place );

						if( row >= 0 )
						{
							beginRemoveRows( parent, row, row );
							p->m_places.removeAt( row );
							endRemoveRows();
						}
					}

					const QModelIndex parent = createIndex(
						d->topRow( &p->m_index ), 1, &p->m_index );

					emit dataChanged( parent, parent );
				}
			}
			else
				d->putNewProductOnPlace( p, place, count );
		}
	}
	else
	{
		d->addNewProduct( code, desc );

		if( !place.isEmpty() && count > 0 )
			d->putNewProductOnPlace( d->findProduct( code ), place, count );
	}
}

} /* namespace Stock */
