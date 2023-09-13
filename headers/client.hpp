#ifndef CLIENT_HPP
#define CLIENT_HPP


#define NBR_OF_CMD 10

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
	string  _command;
	string  _options;
	string  _currentOP;
	string 	_serverPassword;
	map<std::string, bool> 	_setFullAccessForTopic;
	map<std::string, bool> 	_setFullAccessInChannel;
	int     _clientSocket;
	map <int, User>         _user;
	map <string, string>    _cmd;
	std::map <std::string, std::string>	_whoIsOP;
	std::map<std::string, std::string>	_passwordChannel;
	std::map<std::string, bool>			_channelBlockedByPassword;


public:
	Client();
	Client(Client const &src);
	Client &operator=(Client const &rhs);
	~Client();

	//USER
	int		addUser(string buffer, int socketUser);
	bool	userCanExecuteCommand(string password, int userSocket, string buffer);
	void	eraseUser(int socketUser);

	//CMD
	void	parsCommands(string buffer);
	void	checkAndExecuteCmd();
	void	nick();
	void	join();
	void	who();
	void	kick();
	void	pass();
	void	privmsg();
	void	part();
	void	topic();
	void	quit();
	void	invite();
	void	mode();

	//OPTIONS MODE
	void	option_o(std::string channel, std::string arg, std::string nickName);
	void	option_k(std::string channel, std::string arg, std::string passWord);
	void	option_i(std::string channel, std::string option);
	void	option_t(std::string channel, std::string option);


	//CHANNEL
	string	extractChannelName(string buffer);
	bool	checkChannelExist(std::string channelname);

	//GET
	string		getCommand() const;
	string		getOptions() const;
	bool		getStatusPasswordClient(int socketUser);
	int			getClientSocket() const;
	bool		getChannelBlockedByPassword(std::string channel);
	std::string		getWhoIsOP_Nick(std::string channel);
	const User&		getUser(int usersocket) const;
	std::string		getServerPassword() const;
	vector<User>	getUserVector() const;
	const std::string	&getPasswordChannel(std::string channel);

	//SET
	void	setClientSocket(int clientSocket);
	void	setServerPassword(std::string password);
	void	setPasswordChannel(const std::string &channel, std::string passWordChannel);


	//TOOLS
	void	sendToClient(int fd, std::string message);
	void	setWhoIsOP(std::string channel, std::string nickname);
	void	printOutput(int numofoption, string message, int options, int fd);
	bool	UserIsOnChannel(std::string user, std::string channel);
	int		getSocketUserWithName(string user);
	bool	passwordVerifier(int socketUser);
	void	erasePasswordChannel(std::string channel);
	void 	eraseChannelUser(string channel, string user);




};

#endif