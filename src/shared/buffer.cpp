
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
