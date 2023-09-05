#include "../headers/client.hpp"

void Client::parsCommands(string buffer, int socketUser){
	(void)socketUser;
//	cout << "UPPER AVANT = " << buffer << endl;
//	transform(buffer.begin(), buffer.end(), buffer.begin(), ::toupper);
//	cout << "UPPER APRES = " << buffer << endl;
	printOutput(1, buffer, 0, socketUser);
	if (strncmp(buffer.c_str(), "NICK ", 5) == 0){
		if (buffer.length() < 25){
			size_t space = buffer.find(' ');
			if (space < 5) {
				//string command = buffer.substr(0, 4);
				string nickname = buffer.substr(space + 1, buffer.length() - space - 3);
				_cmd[NICK] = nickname;
				cout << "nickname = " << nickname << endl;
				nick(socketUser);
			}
		}
		else
			sendToClient(socketUser, "Beaucoup trop long, comme ma b*** !\r\n");
	}
	else if (strncmp(buffer.c_str(), "WHO ", 4) == 0) {
		std::string response2 = ":" + (string)IP_SERV + " 353 " + _user[socketUser].getNickName() + " = #" + _cmd[JOIN] + " :@" + _user[socketUser].getNickName() + "\r\n"
								":" + (string)IP_SERV + " 315" + _user[socketUser].getNickName() +
								":" + (string)IP_SERV + " " + _user[socketUser].getNickName() + " #" + _cmd[JOIN] + " :End of /WHO list.\r\n";
		sendToClient(socketUser, response2);
	}
	else if (strncmp(buffer.c_str(), "JOIN #", 6) == 0) {

		if (buffer.length() < 25){
			size_t space = buffer.find(' ');
			if (space < 5) {
				//string command = buffer.substr(0, 4);
				string channel = buffer.substr(space + 2, buffer.length() - space - 4);
				_cmd[JOIN] = channel;
				cout << "cmd = " << JOIN << " | channel = " << channel << endl;
				join(socketUser);
			}
		}
		else
			sendToClient(socketUser, "Beaucoup trop long, comme ma b*** !\r\n");
	}
	else if (strncmp(buffer.c_str(), "PRIVMSG ", 8) == 0) {

		for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
			//int currentUserSocket = it->first;
			User currentUser = it->second;
			//cout << "it->first = " << it->first << endl;
			if (currentUser.getSocketUser() != socketUser) {
				std::string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
				                       "@localhost " + currentUser.getNickName() + " :" + buffer + "\r\n";

				sendToClient(currentUser.getSocketUser(), response);
			}
		}


	}
}

void Client::join(int socketUser){
	if (_user.find(socketUser) != _user.end()){
		std::map<string, string>::iterator it;
		it = _cmd.begin();
		User checkUser = _user[socketUser];
		if (checkChannelExist(it->second) == false){
			_user[socketUser].setChannelName(_cmd[JOIN]);
			_user[socketUser].setOperator(true);
			cout << _user[socketUser].getNickName() << " is operator !" << endl;
		}

		string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
						  "@localhost " + "JOIN #" + it->second + "\r\n";
		sendToClient(socketUser, response);
		//_user[socketUser] = checkUser;
	}
}

void Client::nick(int socketUser){
	if (_user.find(socketUser) != _user.end()){
			std::map<string, string>::iterator it;
			it = _cmd.begin();
		User checkUser = _user[socketUser];
		string nickname = _cmd[NICK];
		checkUser.setNickName(nickname);
		string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
						  "@localhost " + "NICK :" + nickname + "\r\n";


		sendToClient(socketUser, response);
		_user[socketUser] = checkUser;
	}
}