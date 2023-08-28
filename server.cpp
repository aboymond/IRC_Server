#include "server.hpp"

Server::Server() : 	_socketServer(0),
	_port(0),
	_password("Default"),
	_validPassword(false)
{

};

Server::Server( int port, string password ) : _port(port), _password(password), _validPassword(false) {
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
};

Server::Server( const Server &src ) {
	*this = src;
}

Server::~Server() {};

Server &Server::operator=( const Server &rhs ) {
	if ( this != &rhs )
	{
		this->_socketServer = rhs._socketServer;
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->_validPassword = rhs._validPassword;
	}
	return ( *this );
}

int Server::getPort() const {
	return( this->_port );
}

int Server::getSocketServer() const {
	return ( this->_socketServer );
}

string	Server::getPassword() const {
	return ( this->_password );
}

bool	Server::getValidPassword() const {
	return ( this->_validPassword );
}

const char * Server::BadArgument::what() const throw() {
	return ("Error: need to have 3 arguments: ./ircserv <port> <password>");
}

const char * Server::FailOpeningSocket::what() const throw() {
	return ("Error: impossible to opening socket");
}

std::ostream & operator<<( std::ostream & o, Server const & i)
{
		o <<
		"Socket server Number: " << i.getSocketServer() << "\n"
		"Connect to port: " << i.getPort() << "\n"
		"Password: " << i.getPassword() << "\n"
		"Password status: " << i.getValidPassword() << "\n";
	return (o);
}

