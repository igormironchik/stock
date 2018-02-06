
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

// Stock include.
#include "server.hpp"


namespace Stock {

//
// ServerPrivate
//

class ServerPrivate {
public:
	ServerPrivate( Server * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init();

	//! Parent.
	Server * q;
}; // class ServerPrivate

void
ServerPrivate::init()
{

}


//
// Server
//

Server::Server( QObject * parent )
	:	QTcpServer( parent )
	,	d( new ServerPrivate( this ) )
{
	d->init();
}

Server::~Server()
{
}

void
Server::incomingConnection( qintptr socketDescriptor )
{

}

} /* namespace Stock */
