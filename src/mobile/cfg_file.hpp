
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_CFG_FILE_HPP_INCLUDED
#define STOCK_CFG_FILE_HPP_INCLUDED

// Qt include.
#include <QString>


namespace Stock {

//
// CfgFile
//

//! Helper to read/write configuration.
class CfgFile Q_DECL_FINAL {
public:
	//! Read configuration.
	static bool read( const QString & fileName, QString & pwd, quint16 & port );
	//! Write configuration.
	static void write( const QString & fileName, const QString & pwd, quint16 port );
	//! \return Configuration file name.
	static QString fileName();
}; // class CfgFile

} /* namespace Stock */

#endif // STOCK_CFG_FILE_HPP_INCLUDED
