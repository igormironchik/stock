
/*
	SPDX-FileCopyrightText: 2018-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Stock include.
#include "exceptions.hpp"


namespace Stock {

//
// Exception
//

Exception::Exception( const QString & msg )
	:	std::runtime_error( "error" )
	,	m_msg( msg )
{
}

Exception::~Exception() noexcept
{
}

const QString &
Exception::msg() const noexcept
{
	return m_msg;
}

} /* namespace Stock */
