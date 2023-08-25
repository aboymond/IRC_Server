#ifndef SERVER_HPP
#define SERVER_HPP

// Linux
#include <cstdlib>
#include <cerrno>
#include <cstring>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <exception>
#include <fcntl.h>
#include <sys/select.h>
#include "user.hpp"

using namespace std;

#define IP_SERV "127.0.0.1"

//#define IP_SERV "10.11.12.1" // IP 42 Alex
//#define IP_SERV "10.11.12.3" // IP 42 Quent


class Server {

private:
	int					_socketServer;
	int 				_port;
	string 				_password;

public:

	Server();
	Server( Server const &src );
	Server( int port, string password );
	Server &operator=( Server const &rhs );
	~Server();


	int 	getPort();
	int 	getSocketServer();
	string	getPassword();


	class BadArgument : public exception
	{
		public:
			virtual const char * what() const throw();
	};

	class FailOpeningSocket : public exception
	{
	public:
		virtual const char * what() const throw();
	};
};

#endif