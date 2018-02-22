
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

#ifndef STOCK_BUFFER_HPP_INCLUDED
#define STOCK_BUFFER_HPP_INCLUDED

// Qt include.
#include <QByteArray>


namespace Stock {

//
// Buffer
//

//! Buffer of the socket.
class Buffer Q_DECL_FINAL {
public:
	Buffer();

	//! \return Data of the buffer.
	const QByteArray & data() const;

	//! Write data to the end of the buffer.
	void write( const QByteArray & data );

	//! Remove first bytes from the beginning of the buffer.
	void remove( int bytes );

	//! Clear the buffer.
	void clear();

	//! Is buffer empty?
	bool isEmpty() const;

	//! \return Size of the buffer.
	int size() const;

private:
	//! Data.
	QByteArray m_data;
}; // class Buffer

} /* namespace Stock */

#endif // STOCK_BUFFER_HPP_INCLUDED
