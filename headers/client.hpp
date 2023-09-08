#ifndef CLIENT_HPP
#define CLIENT_HPP

#define NICK    "NICK"
#define JOIN    "JOIN"

#include "user.hpp"
#include "server.hpp"
#include <vector>
#include <map>

using namespace std;

class User;

class Server;

class Client {

private:
	Server  *_server;
	User    *_userPointer;
	string  _command;
	string  _options;
	string  _currentOP;
	int     _clientSocket;
	map <int, User>         _user;
	map <string, string>    _cmd;
	std::map <std::string, std::string>	_whoIsOP;

public:
	Client();
	Client(Client const &src);
	Client &operator=(Client const &rhs);
	~Client();

	//USER
	int addUser(string buffer, int socketUser);
	bool userCanExecuteCommand(string password, int userSocket, string buffer);
	void eraseUser(int socketUser);

	//CMD
	void parsCommands(string buffer);
	void checkAndExecuteCmd();
	void nick();
	void join();
	void who();
	void kick();
	void privmsg();
	void part();

	//CHANNEL
	string extractChannelName(string buffer);
	bool checkChannelExist(std::string channelname);

	//GET
	std::string getWhoIsOP_Nick(std::string channel);
	string getCommand() const;
	string getOptions() const;
	const User& getUser(int usersocket) const;
	vector<User> getUserVector() const;
	bool getStatusPasswordClient(int socketUser);
	int getClientSocket();

	//SET
	void setClientSocket(int clientSocket);

	//TOOLS
	void sendToClient(int fd, std::string message);
	void setWhoIsOP(std::string channel, std::string nickname);
	void printOutput(int numofoption, string message, int options, int fd);



};

#endif