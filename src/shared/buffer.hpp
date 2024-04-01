
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
	void remove( qint64 bytes );

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
