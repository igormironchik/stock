
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

#ifndef STOCK_EXCEPTIONS_HPP_INCLUDED
#define STOCK_EXCEPTIONS_HPP_INCLUDED

// C++ include.
#include <stdexcept>

// Qt include.
#include <QString>


namespace Stock {

//
// Exception
//

//! Exception.
class Exception
	:	public std::runtime_error
{
public:
	explicit Exception( const QString & msg );
	virtual ~Exception() noexcept;

	//! \return Reason.
	const QString & msg() const noexcept;

private:
	//! Reason.
	QString m_msg;
}; // class Exception

} /* namespace Stock */

#endif // STOCK_EXCEPTIONS_HPP_INCLUDED
