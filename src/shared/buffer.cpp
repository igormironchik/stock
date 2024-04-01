
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Como include.
#include "buffer.hpp"


namespace Stock {

//
// Buffer
//

Buffer::Buffer()
{
}

const QByteArray &
Buffer::data() const
{
	return m_data;
}

void
Buffer::write( const QByteArray & data )
{
	m_data.append( data );
}

void
Buffer::remove( qint64 bytes )
{
	if( bytes > 0 )
		m_data.remove( 0, bytes );
}

void
Buffer::clear()
{
	m_data.clear();
}

bool
Buffer::isEmpty() const
{
	return m_data.isEmpty();
}

int
Buffer::size() const
{
	return m_data.size();
}

} /* namespace Stock */
