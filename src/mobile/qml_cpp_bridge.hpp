
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
