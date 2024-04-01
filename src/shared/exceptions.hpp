
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
