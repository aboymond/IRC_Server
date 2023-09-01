#ifndef CLIENT_HPP
#define CLIENT_HPP

#define NICK    "NICK"

#include "user.hpp"
#include "server.hpp"
#include <vector>
#include <map>

using namespace std;

class User;

class Server;

class Client {

private:
	Server *_server;
	User *_userPointer;
	string _command;
	string _options;
	//vector <User> _user;
	map <int, User> _user;
	map <string, string> _cmd;

public:
	Client();

	Client(Client const &src);

	Client &operator=(Client const &rhs);

	~Client();

	string getCommand() const;

	string getOptions() const;

	const User& getUser(int usersocket) const;

	vector <User> getUserVector() const;

	int addUser(string buffer, int socketUser);
	void parsCommands(string buffer, int socketUser);
//	void commandToFunction(string buffer, int socketUser);
//	void join();
	void nick(int socketUser);
	void join(int socketUser);

	void printOutput(int numofoption, string message, int options, int fd);
//	void serverPrintOutput(int numofoption, string message, int options, int fd);

	void eraseUser(int socketUser);

	void sendToClient(int fd, std::string message);

};


#endif