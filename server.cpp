#include "server.hpp"

Server::Server() {};

Server::Server( int port, string password ) : _port(port), _password(password) {};

Server::Server( const Server &src ) {
	*this = src;
}

Server::~Server() {};

Server &Server::operator=( const Server &rhs ) {
	if ( this != &rhs )
	{
		this->_port = rhs._port;
		this->_socketServer = rhs._socketServer;
		this->_password = rhs._password;
	}
	return ( *this );
}

int Server::getPort() {
	return( this->_port );
}

int Server::getSocketServer() {
	return ( this->_socketServer );
}

string	Server::getPassword() {
	return ( this->_password );
}

const char * Server::BadArgument::what() const throw() {
	return ("Error: need to have 3 arguments: ./ircserv <port> <password>");
}

const char * Server::FailOpeningSocket::what() const throw() {
	return ("Error: impossible to opening socket");
}


