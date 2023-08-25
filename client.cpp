#include "client.hpp"
#include "server.hpp"

Client::Client() {};

Client::Client( const Client &src ) {
	*this = src;
}

Client &Client::operator=( const Client &rhs ) {
	if ( this != &rhs )
	{
		this->_command = rhs._command;
		this->_options = rhs._options;
		this->_server = rhs._server;
		this->_user = rhs._user;
	}
	return ( *this );
}

Client::~Client() {};
