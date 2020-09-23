
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
