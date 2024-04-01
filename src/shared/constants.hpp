
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef STOCK_CONSTANTS_HPP_INCLUDED
#define STOCK_CONSTANTS_HPP_INCLUDED

// Qt include.
#include <QtGlobal>


namespace Stock {

//! Magic number to identify protocol.
static const quint64 c_magic = 0x53544F434B4D5347;
//! UDP port.
static const quint16 c_udpPort = 33663;

} /* namespace Stock */

#endif // STOCK_CONSTANTS_HPP_INCLUDED
