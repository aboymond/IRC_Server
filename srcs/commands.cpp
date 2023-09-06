#include "../headers/client.hpp"

void Client::parsCommands(string buffer, int socketUser){
	(void)socketUser;

	printOutput(1, buffer, 0, socketUser);
	if (strncmp(buffer.c_str(), "NICK ", 5) == 0){
		if (buffer.length() < 25){
			size_t space = buffer.find(' ');
			if (space < 5) {
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
		if (_user[socketUser].getWho() == true) {
			map<int, User>::iterator it;
			vector<string>::iterator it_channel;
			std::string resp_who;
			cout << "TEEEEEEEEST in WHOOOOOO" << endl;
			for (it = _user.begin(); it != _user.end(); it++) {

				User &currentUser = it->second;
				if (_whoIsOP[_cmd[JOIN]] == currentUser.getNickName()) {
					resp_who = ":" + (string) IP_SERV + " 354 " + _user[socketUser].getNickName() + " 152 #" + _cmd[JOIN] +
										   " " + currentUser.getNickName() + " :H@\r\n";
					sendToClient(socketUser, resp_who);
					resp_who.erase();
				}
				else {
					resp_who = ":" + (string) IP_SERV + " 354 " + _user[socketUser].getNickName() + " 152 #" + _cmd[JOIN] +
										   " " + currentUser.getNickName() + " :H\r\n";
					sendToClient(socketUser, resp_who);
					resp_who.erase();
				}
			}
			resp_who =  ":" + (string) IP_SERV + " 315 " + _user[socketUser].getNickName() + " #" + _cmd[JOIN] + " :End of /WHO list.\r\n";
			sendToClient(socketUser, resp_who);
		}
		else {
			if (_whoIsOP[_cmd[JOIN]] == _user[socketUser].getNickName()) {
				std::string response4 =
						":" + (string) IP_SERV + " 353 " + _user[socketUser].getNickName() + " = #" + _cmd[JOIN] +
						" :@" + _user[socketUser].getNickName() + "\r\n"
																  ":" + (string) IP_SERV + " 315 " +
						_user[socketUser].getNickName() + " #" + _cmd[JOIN] + " :End of /WHO list.\r\n";
				sendToClient(socketUser, response4);
			} else {
				std::string response4 =
						":" + (string) IP_SERV + " 353 " + _user[socketUser].getNickName() + " = #" + _cmd[JOIN] +
						" :@" + _whoIsOP[_cmd[JOIN]] + " " + _user[socketUser].getNickName() + "\r\n"
																							   ":" + (string) IP_SERV +
						" 315 " + _user[socketUser].getNickName() + " #" + _cmd[JOIN] + " :End of /WHO list.\r\n";
				sendToClient(socketUser, response4);
			}
			_user[socketUser].setWho(true);

		}
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

				User currentUser = it->second;
				if (currentUser.getSocketUser() != socketUser) {
					std::string response =
							":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
							"@localhost " + buffer + "\r\n";

					sendToClient(currentUser.getSocketUser(), response);
				}
			}
	}
	else if (strncmp(buffer.c_str(), "KICK #", 6) == 0)
	{
//		std::string input = ":USER1!~USER1@localhost KICK #testtest USER1 :USER1";

		map<int, User>::iterator it;
		vector<string>::iterator it_channel;
		string channel = extractChannelName(buffer);
		string commandAndChannel = "KICK #" + channel;
//		cout << commandAndChannel << endl;
//		string response = ":USER1!~USER1@localhost KICK #testtest USER1 :USER1\r\n";
		size_t found = buffer.find_last_of(' ');
		string userToKick = buffer.substr(found+1);
		string userToKick2 = userToKick.erase(userToKick.length()-2);
		string response = ":" + userToKick + "!~" + userToKick + "@localhost " + commandAndChannel + " " + userToKick + " :" + userToKick + "\r\n";
		cout << "response = " << response;
//		cout << "userToKick = " << userToKick << endl;
		if (checkChannelExist(channel) == true)
		{
			for (it = _user.begin(); it != _user.end(); it++)
			{
				User &currentUser = it->second;
				for (it_channel = currentUser.getChannelName().begin(); it_channel < currentUser.getChannelName().end(); ++it_channel) {
					if (*it_channel == "testtest") {
//						cout << "test" << endl;
						sendToClient(socketUser, response);
					}
				}
				sendToClient(socketUser, "there is no channel\r\n");
			}

		}
	}
}

string Client::extractChannelName(string buffer) {
	//Recherchez le caractère '#' dans la chaîne
	size_t found = buffer.find('#');
	found += 1;
	//Trouvé le caractère '#', maintenant, extrayez le nom du canal
	string channel = buffer.substr(found, buffer.find(' ', found) - found);
	return (channel);
}

void Client::join(int socketUser){
	if (_user.find(socketUser) != _user.end()){
		std::map<string, string>::iterator it;
		it = _cmd.begin();
		User checkUser = _user[socketUser];
		if (checkChannelExist(it->second) != true){

			_user[socketUser].setChannelName(_cmd[JOIN]);
			_user[socketUser].setOperator(true);
			setWhoIsOP(_cmd[JOIN], _user[socketUser].getNickName());
			cout << _user[socketUser].getNickName() << " is operator !" << endl;
		}
		for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
			User currentUser = it->second;
			string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
							  "@localhost " + "JOIN #" + _cmd[JOIN] + "\r\n";
			sendToClient(currentUser.getSocketUser(), response);
		}

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