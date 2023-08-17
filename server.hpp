#ifndef SERVER_HPP
#define SERVER_HPP

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

using namespace std;

class Server {

private:
	int					_port;
	string 				_password;
	vector<int>			_idUser;
	int 				_socketServer;
	struct sockaddr_in  _addressServer;
	vector<int> 		_clientSockets;
	//struct sockaddr_in	_addressClient;

public:

	Server();
	Server( Server const &src );
	Server( int port, string password );
	Server &operator=( Server const &rhs );
	~Server();


	int 	getPort();
	int 	getSocketServer();

	void 	setSocketServer( void );
	void 	setAddressServer( void );
	void 	setAndAssignSocketToClient( void );

	void 	connectServer( void );

	//void	startListening();

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

// Test 2 to main
// Test 3 git
// add comment
// add 2
// pouet

#endif