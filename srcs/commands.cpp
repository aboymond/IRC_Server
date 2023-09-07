#include "../headers/client.hpp"

void Client::parsCommands(string buffer) {

	std::stringstream ss(buffer);
	std::string command;
	std::string argument;
	ss >> command;
	std::getline(ss, argument);

	size_t pos = argument.find("\r\n");
	if (pos != std::string::npos) {
		argument.erase(pos);
	}

	_cmd[command] = argument;

	std::map<std::string, std::string>::iterator it = _cmd.find(command);
	if (it != _cmd.end())
		cout << "COMMAND = " << it->first << " | ARGUMENT = " << it->second << endl;
}

void   Client::checkAndExecuteCmd() {
	int totalOfCmd = 5;
	bool commandFound = false;
	std::string command;
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	if (it != _cmd.end())
		command = it->first;
	for (size_t i = 0; i < command.length(); ++i)
		command[i] = std::toupper(command[i]);

	std::string	cmd[totalOfCmd] = { "NICK", "JOIN", "WHO", "KICK", "PRIVMSG" };
	void (Client::*ptr_command[totalOfCmd]) (void) = { &Client::nick, &Client::join, &Client::who, &Client::kick,
											  &Client::privmsg };
	for (int i = 0; i < totalOfCmd; i++) {
		if (cmd[i] == command) {
			(this->*ptr_command[i])();
			commandFound = true;
		}
	}
	if (commandFound == false)
		sendToClient(getClientSocket(), "Command not found: " + command + "\r\n");
	_cmd.clear();
}


//	printOutput(1, buffer, 0, socketUser);
//	if (strncmp(buffer.c_str(), "NICK ", 5) == 0){

//		else
//			sendToClient(socketUser, "Beaucoup trop long, comme ma b*** !\r\n");
//	}
//	else if (strncmp(buffer.c_str(), "WHO ", 4) == 0) {
//		if (_user[socketUser].getWho() == true) {
//			map<int, User>::iterator it;
//			vector<string>::iterator it_channel;
//			std::string resp_who;
//			cout << "TEEEEEEEEST in WHOOOOOO" << endl;
//			for (it = _user.begin(); it != _user.end(); it++) {
//				User &currentUser = it->second;
//				if (_whoIsOP[_cmd[JOIN]] == currentUser.getNickName()) {
//					resp_who = ":" + (string) IP_SERV + " 354 " + _user[socketUser].getNickName() + " 152 #" + _cmd[JOIN] +
//							" " + currentUser.getNickName() + " :H@\r\n";
//					sendToClient(socketUser, resp_who);
//					resp_who.erase();
//				}
//				else {
//					resp_who = ":" + (string) IP_SERV + " 354 " + _user[socketUser].getNickName() + " 152 #" + _cmd[JOIN] +
//							" " + currentUser.getNickName() + " :H\r\n";
//					sendToClient(socketUser, resp_who);
//					resp_who.erase();
//				}
//			}
//			resp_who =  ":" + (string) IP_SERV + " 315 " + _user[socketUser].getNickName() + " #" + _cmd[JOIN] + " :End of /WHO list.\r\n";
//			sendToClient(socketUser, resp_who);
//		}
//		else {
//			if (_whoIsOP[_cmd[JOIN]] == _user[socketUser].getNickName()) {
//				std::string response4 =
//						":" + (string) IP_SERV + " 353 " + _user[socketUser].getNickName() + " = #" + _cmd[JOIN] +
//							" :@" + _user[socketUser].getNickName() + "\r\n"
//								  ":" + (string) IP_SERV + " 315 " + _user[socketUser].getNickName() +
//									" #" + _cmd[JOIN] + " :End of /WHO list.\r\n";
//				sendToClient(socketUser, response4);
//			}
//			else {
//				std::string response4 =
//						":" + (string) IP_SERV + " 353 " + _user[socketUser].getNickName() + " = #" + _cmd[JOIN] +
//							" :@" + _whoIsOP[_cmd[JOIN]] + " " + _user[socketUser].getNickName() + "\r\n"
//									":" + (string) IP_SERV +" 315 " + _user[socketUser].getNickName() +
//										" #" + _cmd[JOIN] + " :End of /WHO list.\r\n";
//				sendToClient(socketUser, response4);
//			}
//			_user[socketUser].setWho(true);
//		}
//	}
//	else if (strncmp(buffer.c_str(), "JOIN #", 6) == 0) {
//
//		if (buffer.length() < 25){
//			size_t space = buffer.find(' ');
//			if (space < 5) {
//				string channel = buffer.substr(space + 2, buffer.length() - space - 4);
//				_cmd[JOIN] = channel;
//				cout << "cmd = " << JOIN << " | channel = " << channel << endl;
//				join(socketUser);
//			}
//		}
//		else
//			sendToClient(socketUser, "Beaucoup trop long, comme ma b*** !\r\n");
//	}
//	else if (strncmp(buffer.c_str(), "PRIVMSG ", 8) == 0) {
//
//			for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
//
//				User currentUser = it->second;
//				if (currentUser.getSocketUser() != socketUser) {
//					std::string response =
//							":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
//							"@localhost " + buffer + "\r\n";
//
//					sendToClient(currentUser.getSocketUser(), response);
//				}
//			}
//	}
//	else if (strncmp(buffer.c_str(), "KICK #", 6) == 0)
//	{
////		std::string input = ":USER1!~USER1@localhost KICK #testtest USER1 :USER1";
//
//		map<int, User>::iterator it;
//		vector<string>::iterator it_channel;
//		string channel = extractChannelName(buffer);
//		string commandAndChannel = "KICK #" + channel;
////		cout << commandAndChannel << endl;
////		string response = ":USER1!~USER1@localhost KICK #testtest USER1 :USER1\r\n";
//		size_t found = buffer.find_last_of(' ');
//		string userToKick = buffer.substr(found+1);
//		string userToKick2 = userToKick.erase(userToKick.length()-2);
//		string response = ":" + userToKick + "!~" + userToKick + "@localhost " + commandAndChannel + " " + userToKick + " :" + userToKick + "\r\n";
//		cout << "response = " << response;
////		cout << "userToKick = " << userToKick << endl;
//		if (checkChannelExist(channel) == true)
//		{
//			for (it = _user.begin(); it != _user.end(); it++)
//			{
//				User &currentUser = it->second;
//				for (it_channel = currentUser.getChannelName().begin(); it_channel < currentUser.getChannelName().end(); ++it_channel) {
//					if (*it_channel == "testtest") {
////						cout << "test" << endl;
//						sendToClient(socketUser, response);
//					}
//				}
//				sendToClient(socketUser, "there is no channel\r\n");
//			}
//
//		}
//	}




void Client::join(){
	int clientSocket = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string command = it->first;
	std::string channel = it->second;

	if (_user.find(clientSocket) != _user.end()){
		std::map<string, string>::iterator it;
		it = _cmd.begin();
		User checkUser = _user[clientSocket];
		if (checkChannelExist(it->second) != true){

			_user[clientSocket].setChannelName(channel);
			_user[clientSocket].setOperator(true);
			setWhoIsOP(channel, _user[clientSocket].getNickName());
			cout << _user[clientSocket].getNickName() << " is operator !" << endl;
		}
		for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
			User currentUser = it->second;
			string response = ":" + _user[clientSocket].getNickName() + "!~" + _user[clientSocket].getUserName() +
							  "@localhost " + "JOIN " + channel + "\r\n";
			sendToClient(currentUser.getSocketUser(), response);
		}
	}
	_cmd.clear();
}

void Client::nick(){
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string nickname = it->second;

	if (nickname.length() < 8) {
		if (_user.find(socketUser) != _user.end()){
			std::map<string, string>::iterator it;
			it = _cmd.begin();
			User checkUser = _user[socketUser];
			checkUser.setNickName(nickname);
			string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
			                  "@localhost " + "NICK :" + nickname + "\r\n";


			sendToClient(socketUser, response);
			_user[socketUser] = checkUser;
		}
	}
	else {
		sendToClient(socketUser, "Nickname to long, max 8 characters\r\n");
	}
	_cmd.clear();
}

void    Client::who() {
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string channel = it->second;

	if (_user[socketUser].getWho() == true) {
	map<int, User>::iterator it;
	vector<string>::iterator it_channel;
	std::string resp_who;

	for (it = _user.begin(); it != _user.end(); it++) {
		User &currentUser = it->second;
		if (_whoIsOP[channel] == currentUser.getNickName()) {
			resp_who = ":" + (string) IP_SERV + " 354 " + _user[socketUser].getNickName() + " 152 " + channel +
					" " + currentUser.getNickName() + " :H@\r\n";
			sendToClient(socketUser, resp_who);
			resp_who.erase();
		}
		else {
			resp_who = ":" + (string) IP_SERV + " 354 " + _user[socketUser].getNickName() + " 152 " + channel +
					" " + currentUser.getNickName() + " :H\r\n";
			sendToClient(socketUser, resp_who);
			resp_who.erase();
		}
	}
	resp_who =  ":" + (string) IP_SERV + " 315 " + _user[socketUser].getNickName() + " " + channel + " :End of /WHO list.\r\n";
	sendToClient(socketUser, resp_who);
	}
	else {
		if (_whoIsOP[channel] == _user[socketUser].getNickName()) {
			std::string response4 =
					":" + (string) IP_SERV + " 353 " + _user[socketUser].getNickName() + " = " + channel +
						" :@" + _user[socketUser].getNickName() + "\r\n"
							  ":" + (string) IP_SERV + " 315 " + _user[socketUser].getNickName() +
								" " + channel + " :End of /WHO list.\r\n";
			sendToClient(socketUser, response4);
		}
		else {
			std::string response4 =
					":" + (string) IP_SERV + " 353 " + _user[socketUser].getNickName() + " = " + channel +
						" :@" + _whoIsOP[channel] + " " + _user[socketUser].getNickName() + "\r\n"
								":" + (string) IP_SERV +" 315 " + _user[socketUser].getNickName() +
									" " + channel + " :End of /WHO list.\r\n";
			sendToClient(socketUser, response4);
		}
		_user[socketUser].setWho(true);
	}
	_cmd.clear();
}

void    Client::kick(){
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it_chan = _cmd.begin();
	std::string channel = it_chan->second;

	map<int, User>::iterator it;
	vector<string>::iterator it_channel;
	string commandAndChannel = "KICK " + channel;

	size_t found = channel.find_last_of(' ');
	string userToKick = channel.substr(found+1);
	string userToKick2 = userToKick.erase(userToKick.length()-2);
	string response = ":" + userToKick + "!~" + userToKick + "@localhost " + commandAndChannel + " " + userToKick + " :" + userToKick + "\r\n";
	cout << "response = " << response;
	if (checkChannelExist(channel) == true)
	{
		for (it = _user.begin(); it != _user.end(); it++)
		{
			User &currentUser = it->second;
			for (it_channel = currentUser.getChannelName().begin(); it_channel < currentUser.getChannelName().end(); ++it_channel) {
				if (*it_channel == "testtest") {
					sendToClient(socketUser, response);
				}
			}
			sendToClient(socketUser, "There is no channel\r\n");
		}
	}
	_cmd.clear();
}

void    Client::privmsg(){
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it_chan = _cmd.begin();
	std::string command = it_chan->first;
	std::string msg = it_chan->second;
	cout << "MSG = " << msg << endl;

	for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {

		User currentUser = it->second;
		if (currentUser.getSocketUser() != socketUser) {
			std::string response =
					":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
					"@localhost "+ command + msg + "\r\n";

			sendToClient(currentUser.getSocketUser(), response);
		}
	}
	_cmd.clear();
}