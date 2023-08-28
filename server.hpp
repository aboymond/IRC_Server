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
#include <poll.h>
#include <sys/select.h>
#include "user.hpp"
#include "client.hpp"

using namespace std;

#define IP_SERV "127.0.0.1"

//#define IP_SERV "10.11.12.1" // IP 42 Alex
//#define IP_SERV "10.11.12.3" // IP 42 Quent

class User;

class Server {

private:
	int					_socketServer;
	int 				_port;
	string 				_password;
	bool 				_validPassword;
	int 				_userSocket;
	struct sockaddr_in 	_serverAddress;
	std::vector<pollfd>		_pfds;
	//Client				*client;
	//User				*_user;

public:

	Server();
	Server( Server const &src );
	Server( int port, string password );
	Server &operator=( Server const &rhs );
	~Server();


	int 	getPort() const;
	int 	getSocketServer() const;
	string	getPassword() const;
	bool	getValidPassword() const;
	void 	createSocketServer();

	void	waitToNewConnection();


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

	std::ostream &	operator<<( std::ostream & o, Server const & i );
#endif