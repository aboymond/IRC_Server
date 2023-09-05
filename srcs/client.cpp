#include "../headers/client.hpp"
#include "../headers/server.hpp"

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

//const User& Client::getUser(int usersocket) const {
//	for (size_t i = 0; i < _user.size(); i++) {
//		const User &checkUser = _user[i];
//		cout << "usersocket = " << usersocket << "_user = " << checkUser.getSocketUser() << endl;
//		if (checkUser.getSocketUser() == usersocket) {
//			return (_user[i]);
//		}
//	}
//	//return (_user[i]);
//
//	throw std::runtime_error("User not found");
//
//}

//vector <User> Client::getUserVector() const {
//	return (this->_user);
//}

string Client::getCommand() const {
	return (this->_command);
}

string Client::getOptions() const {
	return (this->_options);
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

bool Client::GetStatusPasswordClient(int socketUser) {
	return (_user[socketUser].getPasswordIsValid());
}