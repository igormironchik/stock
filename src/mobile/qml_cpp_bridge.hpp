
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

#ifndef STOCK_QML_CPP_BRIDGE_HPP_INCLUDED
#define STOCK_QML_CPP_BRIDGE_HPP_INCLUDED

// Qt include.
#include <QObject>

// C++ include.
#include <memory>

// Stock include.
#include "messages.hpp"

QT_BEGIN_NAMESPACE
class QStringListModel;
class QSortFilterProxyModel;
QT_END_NAMESPACE


namespace Stock {

class ListModel;

//
// QmlCppBridge
//

class QmlCppBridgePrivate;

//! Bridge between QML and C++.
class QmlCppBridge Q_DECL_FINAL
	:	public QObject
{
	Q_OBJECT

signals:
	//! Connect request.
	void connectRequest( const QString & pwd, quint16 port );
	//! Disconnect request.
	void disconnectRequest();
	//! Connection established.
	void connectionEstablished();
	//! Disconnected.
	void disconnected( const QString & reason );
	//! Operation successful.
	void opOk();
	//! Operation failed.
	void opFailed();
	//! Put button clicked.
	void putBtnClicked();
	//! Take button clicked.
    void takeBtnClicked();
	//! Search by code button clicked.
    void searchByCodeBtnClicked();
	//! Search by place button clicked.
    void searchByPlaceBtnClicked();
	//! Return back.
	void returnBack();
	//! Put product.
	void putProduct( const QString & code, const QString & place, long long count );
	//! Take product.
	void takeProduct( const QString & code, const QString & place, long long count );
	//! Search.
	void search( const QString & code, const QString & place );
	//! Internal search requested.
	void internalSearch( const QString & data );
	//! List of products received.
	void listReceived();
	//! Show camera.
	void showCamera();
	//! Show camera settings.
	void showCameraSettings();

public:
	explicit QmlCppBridge( const QString & configFileName );
	virtual ~QmlCppBridge();

	//! \return Codes model.
	QSortFilterProxyModel * codesModel() const;
	//! \return Places model.
	QSortFilterProxyModel * placesModel() const;
	//! \return Search model.
	ListModel * searchModel() const;

private slots:
	//! Connect requested.
	void connectRequested( const QString & pwd, quint16 port );
	//! Network connected.
	void connected( const QStringList & codes, const QStringList & places );
	//! Disconnect requested.
	void disconnectRequested();
	//! Server error.
	void serverError();
	//! Network disconnected.
	void networkDisconnected( bool requestedByUser );
	//! Put product.
	void putProductRequested( const QString & code, const QString & place, long long count );
	//! Take product.
	void takeProductRequested( const QString & code, const QString & place, long long count );
	//! Search requested.
	void searchRequested( const QString & code, const QString & place );
	//! List of products received.
	void listOfProductsReceived( const Stock::Messages::ListOfProducts & msg );

private:
	friend class QmlCppBridgePrivate;

	Q_DISABLE_COPY( QmlCppBridge )

	std::unique_ptr< QmlCppBridgePrivate > d;
}; // class QmlCppBridge

} /* namespace Stock */

#endif // STOCK_QML_CPP_BRIDGE_HPP_INCLUDED
