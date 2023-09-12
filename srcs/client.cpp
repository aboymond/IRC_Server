#include "../headers/client.hpp"

Client::Client(){};

Client::Client(const Client &src) {
	*this = src;
}

Client &Client::operator=(const Client &rhs) {
	if (this != &rhs) {
		this->_command = rhs._command;
		this->_options = rhs._options;
		this->_server = rhs._server;
		this->_user = rhs._user;
	}
	return (*this);
}

Client::~Client() {};


string Client::getCommand() const {
	return (this->_command);
}

string Client::getOptions() const {
	return (this->_options);
}

void Client::setWhoIsOP(std::string channel, std::string nickname) {
	_whoIsOP.insert(std::make_pair(channel, nickname));
}

void Client::setServerPassword(std::string password) {
	_serverPassword = password;
}

void Client::setClientSocket(int clientSocket) {
	_clientSocket = clientSocket;
}

int Client::getClientSocket() const{
	return(_clientSocket);
}

std::string Client::getServerPassword() const {
	return(_serverPassword);
}

std::string Client::getWhoIsOP_Nick(std::string channel) {
	string tmp_nick = "";
	if(_whoIsOP.find(channel) != _whoIsOP.end()){
		tmp_nick = _whoIsOP[channel];

	}
	return (tmp_nick);
}

int Client::addUser(string buffer, int socketUser) {
	if (_user.find(socketUser) != _user.end() && _user[socketUser].getUserCreate() == true) {
		//printOutput(1, buffer, 0, socketUser);
		return (-1);
	}
	User newUser;

	if (newUser.initUserAndNick(buffer) == true)
		_user[socketUser].setUserCreate(true);
	newUser.setSocketUser(socketUser);
	_user[socketUser] = newUser;
	//printOutput(1, buffer, 0, socketUser);
	return (0);
}

void Client::eraseUser(int socketUser){
	if(_user.find(socketUser) != _user.end()){
		_user.erase(socketUser);
	}
}

bool Client::userCanExecuteCommand(string password, int userSocket, string buffer) {
	int deletechar = buffer.length();
	buffer = buffer.substr(0, deletechar - 2);
	if (password == buffer)
	{
		_user[userSocket].setPasswordIsValid(true);
		sendToClient(userSocket, "Password valid. You can execute some commands\r\n");
		return (true);
	}
	return (false);
}

bool Client::checkChannelExist(std::string channelName) {
	std::map<int, User>::iterator it;
	for (it = _user.begin(); it != _user.end(); ++it) {
		const User& currentUser = it->second;
		if (currentUser.searchChannel(channelName)) {
			return true;
		}
	}
	return false;
}

bool Client::getStatusPasswordClient(int socketUser) {
	return (_user[socketUser].getPasswordIsValid());
}

string Client::extractChannelName(string buffer) {

	size_t found = buffer.find('#');
//	found += 1;

	string channel = buffer.substr(found, buffer.find(' ', found) - found);
	return (channel);
}

bool Client::passwordVerifier(int socketUser) {

	if (getStatusPasswordClient(socketUser) == false) {
		return (false);
	}
	else
		return (true);
}

bool Client::UserIsOnChannel(std::string user, std::string channel) {
	for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
		if (it->second.getNickName() == user) {
			vector <string> channelUser = it->second.getChannelName();
			for (size_t i = 0; i < channelUser.size(); ++i) {
				if (channelUser[i] == channel) {
					cout << "user is on channel" << endl;
					return (true);
				}
			}
			cout << "user is not in this channel" << endl;
			return (false);
		}
	}
	return false;
}

int Client::getSocketUserWithName(string user) {
	for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
		if (it->second.getNickName() == user)
		{
			int socketUserInvited = it->first;
			return (socketUserInvited);
		}
	}
	return (0);
}

