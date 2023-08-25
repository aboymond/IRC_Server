#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "client.hpp"
#include "user.hpp"
#include "server.hpp"
#include <vector>

class Client {

private:
	Server 			*_server;
	vector<User>	_user;
	string			_command;
	string 			_options;

public:
	Client();
	Client( Client const &src );
	Client &operator=( Client const &rhs );
	~Client();

};


#endif