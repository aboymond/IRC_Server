#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "user.hpp"
#include "server.hpp"
#include <vector>

using namespace std;

class User;
class Server;

class Client {

private:
	Server 			*_server;
    User            *_userPointer;
	string			_command;
    string 			_options;
    vector<User>	_user;

public:
    Client();
    Client( Client const &src );
	Client &operator=( Client const &rhs );
	~Client();

	string getCommand() const;
	string getOptions() const;
    vector<User>    getUserVector() const;

    void    addUser(string buffer, int socketUser);

};


#endif