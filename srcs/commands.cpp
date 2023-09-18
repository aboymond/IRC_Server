#include "../headers/client.hpp"

void Client::parsCommands(string buffer) {

	if (passwordVerifier(getClientSocket()) != true ) {
		addBufferToTmpVector(buffer);
	}
	std::stringstream ss(buffer);
	std::string command;
	std::string argument;
	ss >> command;
	std::getline(ss, argument);
	size_t pos = argument.find('\r');

	cout << "COMMAND avant = " << command << " | ARGUMENT avant = " << argument << endl;

	if (pos != string::npos) {
		argument.erase(pos);
	}

//	argument.erase(argument.length() - 1);
	argument.erase(0, 1);

	_cmd[command] = argument;


	std::map<std::string, std::string>::iterator it = _cmd.find(command);
	if (it != _cmd.end())
		cout << "COMMAND = " << it->first << " | ARGUMENT = " << it->second << endl;
}

void   Client::checkAndExecuteCmd() {
	bool commandFound = false;
	std::string command;
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	if (it != _cmd.end())
		command = it->first;
	for (size_t i = 0; i < command.length(); ++i)
		command[i] = std::toupper(command[i]);

	cout << "dans check command" << endl;



	if (passwordVerifier(getClientSocket()) == true || command == "PASS") {
		std::string	cmd[NBR_OF_CMD] = { "CAP LS", "USER", "NICK", "JOIN", "WHO", "KICK", "PRIVMSG", "PASS", "PART", "TOPIC", "INVITE", "MODE" };
		void (Client::*ptr_command[NBR_OF_CMD]) (void) = { &Client::capls,&Client::user,&Client::nick, &Client::join, &Client::who, &Client::kick,
												  &Client::privmsg, &Client::pass, &Client::part, &Client::topic, &Client::invite, &Client::mode };
		for (int i = 0; i < NBR_OF_CMD; i++) {

			if (cmd[i] == command) {
				(this->*ptr_command[i])();
				commandFound = true;
			}
		}
		if (commandFound == false)
			sendToClient(getClientSocket(), "Command not found: " + command + "\r\n");
	}



	_cmd.clear();
}

void Client::join(){
	int clientSocket = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string channel = it->second;
	std::string passWord;

	if (!channel.empty()) {
		std::stringstream ss(channel);
		ss >> channel >> passWord;
		passWord.erase(passWord.length());
		cout << "Password for channel = " << passWord <<endl;
		if (getPasswordChannel(channel) == passWord)
			_user[clientSocket].setUserHaveGoodPassForEnterInChannel(channel, true);
		else if (getPasswordChannel(channel) != passWord && _channelBlockedByPassword[channel] == true) {
			sendToClient(_user[clientSocket].getSocketUser(), "Bad password\r\n");
			_user[clientSocket].setUserHaveGoodPassForEnterInChannel(channel, false);
		}
	}


	if (_channelBlockedByPassword[channel] == false || (_channelBlockedByPassword[channel] == true && _user[clientSocket].getUserHaveGoodPassForEnterInChannel(channel) == true) ) {
		if (_setFullAccessInChannel[channel] != true || _user[clientSocket].getAccessWithInvite(channel) == true) {
			if (_user.find(clientSocket) != _user.end()) {
				std::map<string, string>::iterator it;
				it = _cmd.begin();
				if (checkChannelExist(channel) != true) {

					_user[clientSocket].setChannelName(channel);
					_user[clientSocket].setOperator(true);
					_user[clientSocket].setIsOperator(channel, true);
					setWhoIsOP(channel, _user[clientSocket].getNickName());
					_user[clientSocket].setWho(false);
					cout << _user[clientSocket].getNickName() << " is operator !" << endl;
				} else {
					_user[clientSocket].setChannelName(channel);
					_user[clientSocket].setIsOperator(channel, false);
					_user[clientSocket].setWho(false);
				}
				for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
					User currentUser = it->second;
					if (currentUser.searchChannel(channel) == true) {
						std::string responseToJoin =
								":" + _user[clientSocket].getNickName() + "!~" + _user[clientSocket].getUserName() +
								"@localhost " + "JOIN " + ":" + channel + "\r\n";

						sendToClient(currentUser.getSocketUser(), responseToJoin);
					}

				}
			}

		}
		else {
			sendToClient(_user[clientSocket].getSocketUser(), ":localhost 473 " + _user[clientSocket].getNickName() + " " + channel +
				":Cannot join channel (invite only)\r\n");
		}
	}
	else {
		std::string responseBadPassword =
				":localhost 475 " + _user[clientSocket].getNickName() + " " + channel + " :Cannot join channel (incorrect channel key)\r\n";

		sendToClient(_user[clientSocket].getSocketUser(), responseBadPassword);
	}
	_user[clientSocket].printAllChannel();
	_cmd.clear();
}

void Client::nick(){
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string nickname = it->second;

	if (nickname.length() < 15) {
		if (_user.find(socketUser) != _user.end()){
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
	std::string resp_who;
	std::string op;
	std::string allUserInChan;

	if (_user[socketUser].getWho() == true) {
		map<int, User>::iterator it;

		for (it = _user.begin(); it != _user.end(); it++) {
			User &currentUser = it->second;
			cout << "IN WHO NAME = " << currentUser.getNickName() << " | channel = " << channel << " | isOP = " << currentUser.getIsOperator(channel) << endl;

			if (currentUser.searchChannel(channel)) {

				if (currentUser.getIsOperator(channel) == true)
					op = "@";
				else
					op = "";

				resp_who = ":*.localhost 354 " + _user[socketUser].getNickName() + " 152 " + channel +
						   " " + currentUser.getNickName() + " :H" + op + "\r\n";
				sendToClient(socketUser, resp_who);
				resp_who.erase();
			}

		}
		resp_who =  ":*.localhost 315 " + _user[socketUser].getNickName() + " " + channel + " :End of /WHO list.\r\n";
		sendToClient(socketUser, resp_who);
		resp_who.erase();
		op.erase();
	}
	else {

		for (std::map<int, User>::iterator it_userInChan = _user.begin(); it_userInChan != _user.end(); it_userInChan++) {
			User &currentuser = it_userInChan->second;
			if (currentuser.userIsOnChannelWithTopic(channel))
			{
				string response = ":*.localhost 332 " + _user[socketUser].getNickName() + " " + channel + " " + currentuser.getChannelTopic(channel) + "\r\n";
				sendToClient(socketUser, response);
			}
			if (it_userInChan->second.searchChannel(channel) == true) {
				if (it_userInChan->second.getIsOperator(channel) == true)
					allUserInChan += "@" + it_userInChan->second.getNickName() + " ";
				else
					allUserInChan += it_userInChan->second.getNickName() + " ";

			}

		}

		resp_who = ":*.localhost 353 " + _user[socketUser].getNickName() + " = " + channel +
				   " :" + op + allUserInChan + "\r\n"
																 ":*.localhost 366 " + _user[socketUser].getNickName() + " " + channel + " :End of /NAMES list.\r\n";
		sendToClient(socketUser, resp_who);
		resp_who.erase();
		for (std::map<int, User>::iterator it_user = _user.begin(); it_user != _user.end(); it_user++) {
			User &currentUser = it_user->second;
			if (currentUser.searchChannel(channel) == true) {
				std::string addUserToAnotherChannel;
				if (currentUser.getIsOperator(channel) == true)
					addUserToAnotherChannel = ":*.localhost 352 " + _user[socketUser].getNickName() + " " + channel + " ~" + currentUser.getUserName() + " localhost localhost " + currentUser.getNickName() + " H@ :0\r\n";
				else
					addUserToAnotherChannel = ":*.localhost 352 " + _user[socketUser].getNickName() + " " + channel + " ~" + currentUser.getUserName() + " localhost localhost " + currentUser.getNickName() + " H :0\r\n";
				sendToClient(_user[socketUser].getSocketUser(), addUserToAnotherChannel);
			} else
				continue;

		}
		resp_who = ":*.localhost 315 " + _user[socketUser].getNickName() +
				   " " + channel + " :End of /WHO list.\r\n";

		sendToClient(socketUser, resp_who);
		resp_who.erase();
		op.erase();
		_user[socketUser].setWho(true);
	}
	_cmd.clear();
}


void    Client::kick(){
	int socketClient = getClientSocket();
	map<string, string>::iterator argument = _cmd.begin();

	string nickname = _user[socketClient].getNickName();
	string username = _user[socketClient].getUserName();
	size_t space = argument->second.find(' ');

	string userToKick = argument->second.substr(space + 1, argument->second.length());
	string responseNoSuchChannel = ":*.localhost 403 " + nickname + " " + "localhost" + " :No such channel\r\n";


	string channel = extractChannelName(argument->second);
	if (channel == "NULL") {
		sendToClient(socketClient, responseNoSuchChannel);
		_cmd.clear();
		return;
	}
	string responseIfUserCanKick = ":" + nickname + "!~" + username + "@localhost KICK " + channel + " " + userToKick + " :" + nickname + "\r\n";
	string responseIfUserNotExistInChannel = ":localhost 401 " + nickname + userToKick + " :No such Nick\r\n";
	string responseIfUserIsNotOperator = ":*.localhost 482 " + nickname + " " + channel + " :You must be a channel half-operator\r\n";

	std::map<int, User>::iterator it;
	if (_user[socketClient].getIsOperator(channel))
	{
		if (UserIsOnChannel(userToKick, channel))
		{
			for (it = _user.begin();  it != _user.end() ; it++) {
				if (it->second.getNickName() == userToKick)
				{
					_user[it->second.getSocketUser()].delChannelName(channel);
				}
				sendToClient(it->second.getSocketUser(), responseIfUserCanKick);
			}
		}
		else
			sendToClient(socketClient, responseIfUserNotExistInChannel);
	}
	else
		sendToClient(socketClient, responseIfUserIsNotOperator);
	_cmd.clear();
}

void    Client::part(){

	int socketClient = getClientSocket();
	map<string, string>::iterator argument = _cmd.begin();

	string nickname = _user[socketClient].getNickName();
	string username = _user[socketClient].getUserName();

	string channel = extractChannelName(argument->second);
	string responseIfUserCanLeaveChannel = ":" + nickname + "!~" + username + "@localhost PART " + channel + " :\"Leaving\"\r\n";
	string responseIfChannelNotExist = ":*.localhost 403 " + nickname + " " + channel + " :No such channel\r\n";

	std::map<int, User>::iterator it;
		if (checkChannelExist(channel))
		{
			for (it = _user.begin();  it != _user.end() ; it++) {
				if (it->second.getNickName() == nickname)
				{
					_user[it->second.getSocketUser()].delChannelName(channel);
				}
				sendToClient(it->second.getSocketUser(), responseIfUserCanLeaveChannel);
			}
		}
		else
			sendToClient(socketClient, responseIfChannelNotExist);
	_cmd.clear();
}

void    Client::topic(){
	int socketUser = getClientSocket();
	map<std::string, std::string>::iterator it_argument = _cmd.begin();
	string argument = it_argument->second;
	string nick = _user[socketUser].getNickName();
	string user = _user[socketUser].getUserName();
	string channel = argument.substr(0, argument.find(' '));
	size_t found = argument.find(' ');
	if (found == string::npos)
	{
		string response = ":*.localhost 331 " + nick + " " + channel + " :No topic is set.\r\n";
		sendToClient(socketUser, response);
	}
	else
	{
		string nameOfChannelTopic = argument.substr(argument.find(':'), argument.length());

		string responseIfchannelNotExiste = ":*.@localhost 403 " + nick + " " + nameOfChannelTopic + " :No such channel\r\n";
		string responseIfChannelCanHaveTop = ":" + nick + "!~" + user + "@localhost" + " TOPIC " + argument + "\r\n";
		string responseIfUserHaveNoPermission = ":*.localhost 482 " + nick + " " + channel + " :You do not have access to change the topic on this channel\r\n";
		if (checkChannelExist(channel) == true)
		{
			if (_user[socketUser].getIsOperator(channel) == true || _setFullAccessForTopic[channel] == true)
			{
				for (map<int, User>::iterator it = _user.begin(); it != _user.end() ; ++it) {
//                    cout << "username = " << it->second.getUserName() << endl;
					User currentUser = it->second;
					if (currentUser.searchChannel(channel) == true) {
						string user = _user[socketUser].getUserName();
						string nick = _user[socketUser].getNickName();
						string response2 = ":" + nick + "!~" + user + "@localhost TOPIC " + argument + "\r\n";
						sendToClient(currentUser.getSocketUser(), response2);
//						_user[socketUser].setChannelandTopic(true, channel, nameOfChannelTopic);
						_user[socketUser].setChannelandTopic(channel, nameOfChannelTopic);
//						_user[socketUser].printChannelTopic();
					}
				}
//                sendToClient(socketUser, responseIfChannelCanHaveTop);
			}
			else
			{
				sendToClient(socketUser, responseIfUserHaveNoPermission);
			}
		}
		else
			sendToClient(socketUser, responseIfchannelNotExiste);

	}
	_cmd.clear();
}

void    Client::privmsg(){
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it_chan = _cmd.begin();
	std::string msg = it_chan->second;
	cout << "MSG = " << msg << endl;

	for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {

		User currentUser = it->second;
		if (currentUser.getSocketUser() != socketUser) {
			std::string response =
					":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
					"@localhost " + "PRIVMSG " + msg + "\r\n";


			sendToClient(currentUser.getSocketUser(), response);
		}
	}
	_cmd.clear();
}

void Client::pass(){

	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string command = it->first;
	std::string password = it->second;

	for (size_t i = 0; i < command.length(); ++i)
		command[i] = std::toupper(command[i]);

	cout << "in pass command = " << command << " | password = " << password << " | getPass = " << getServerPassword() << endl;
	if (password == getServerPassword()) {
		sendToClient(getClientSocket(), "WELCOME TO THE SERVER\r\n");
		_user[getClientSocket()].setPasswordIsValid(true);
	}
	else {
		sendToClient(getClientSocket(), "Error bad password: " + password + "\r\n");
		sendToClient(getClientSocket(), "Enter the password with /PASS\r\n");
	}
	_cmd.clear();
}

void Client::quit() {
	int socketUser = getClientSocket();

	for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {

		User currentUser = it->second;
		if (currentUser.getSocketUser() != socketUser) {
			std::string response =
					":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
					"@localhost " + "QUIT :Quit: Leaving\r\n";


			sendToClient(currentUser.getSocketUser(), response);
		}
	}
	eraseUser(socketUser);
	_cmd.clear();
}

void Client::invite() {
	int socketUser = getClientSocket();

	std::map<string, string>::iterator itArg = _cmd.begin();

	string userWhoInvite = _user[socketUser].getNickName();
	string argument = itArg->second;
	string userToInvite = argument.substr(0, argument.find(' '));
	string channel = argument.substr(argument.find('#'), argument.length());

	string reponseIfUsercanBeInvited = ":" + userWhoInvite + "!~" + userWhoInvite + "@localhost" + " INVITE " + userToInvite + " " + channel + "\r\n";
	string responseIfUserCanNotInvited = ":*.localhost 482 " + userWhoInvite + " " + channel + " :You must be a channel half-operator\r\n";
	string InvitatitionIsDone = ":*.localhost 341 " + userWhoInvite + " " + userToInvite + " " + channel + "\r\n";
	int socketUserInvite = getSocketUserWithName(userToInvite);
	if (_user[socketUser].getIsOperator(channel) == true)
	{
		if (UserIsOnChannel(userToInvite, channel) == false) {
			_user[socketUserInvite].setAccessWithInvite(channel, true);
			sendToClient(socketUserInvite, reponseIfUsercanBeInvited);
			sendToClient(socketUser, InvitatitionIsDone);
		}
	}
	else
		sendToClient(socketUser, responseIfUserCanNotInvited);

	_cmd.clear();
}

void Client::mode() {
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string channel = it->second;
	vector<string>::iterator it_channel;
	std::string option;
	std::string arg2;
	std::stringstream ss(channel);
	ss >> channel >> option;
	if (!option.empty()) {
		ss >> arg2;
		arg2.erase(arg2.length());
	}

	if (_user[socketUser].getIsOperator(channel) == true){
		if (option == "-o" || option == "+o") {
			option_o(channel, option, arg2);
			cout << "In mode o = " << channel << " | option = " << option << " | nickname = " << arg2 << endl;
		}
		else if (option == "-k" || option == "+k") {
			option_k(channel, option, arg2);
			cout << "In mode k = " << channel << " | option = " << option << " | nickname = " << arg2 << endl;
		}
		else if (option == "-t" || option == "+t") {
			option_t(channel, option);
		}
		else if (option == "-i" || option == "+i") {
			option_i(channel, option);
		}
		cout << "Bad argument in MODE" << endl;
	}
	else {
		cout << "Argument not found" << endl;
	}
	_cmd.clear();
}

void Client::option_o(std::string channel, std::string option, std::string nickName) {
	int socketUser = getClientSocket();
	std::string arg_resp;

	if (option == "+o")
		arg_resp = "+o";
	else
		arg_resp = "-o";
	if (UserIsOnChannel(nickName, channel) == true){

		for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
			User &currentUser = it->second;
			if (currentUser.searchChannel(channel) == true) {
				if (currentUser.getNickName() == nickName && arg_resp == "+o") {
					cout << "IN +o" << endl;
					currentUser.setWho(false);
					currentUser.setIsOperator(channel, true);
					cout << "IN MODE +O NAME = " << currentUser.getNickName() << " | channel = " << channel << " | isOP = " << currentUser.getIsOperator(channel) << endl;

				}
				else if (currentUser.getNickName() == nickName && arg_resp == "-o") {
					cout << "IN -o" << endl;
					currentUser.setWho(false);
					currentUser.setIsOperator(channel, false);
					cout << "IN MODE -O NAME = " << currentUser.getNickName() << " | channel = " << channel << " | isOP = " << currentUser.getIsOperator(channel) << endl;

				}
				sendToClient(currentUser.getSocketUser(), ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
														  "@localhost " + "MODE " + channel + " " + arg_resp + " :" + nickName +"\r\n" );
			}

		}
	}
}

void Client::option_k(std::string channel, std::string option, std::string passWord) {
	int socketUser = getClientSocket();
	std::string arg_resp;

	if (option == "+k")
		arg_resp = "+k";
	else
		arg_resp = "-k";

	if (arg_resp == "+k") {
		setPasswordChannel(channel, passWord);
	}
	else if (arg_resp == "-k" && getPasswordChannel(channel) == passWord) {
		erasePasswordChannel(channel);
	}
	sendToClient(_user[socketUser].getSocketUser(), ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
			  "@localhost " + "MODE " + channel + " " + arg_resp + " :" + passWord +"\r\n" );
}

void Client::option_t(std::string channel, std::string option) {
	int socketUser = getClientSocket();
	std::string arg_resp;

	if (option == "+t")
		arg_resp = "+t";
	else
		arg_resp = "-t";

	if (arg_resp == "+t") {
		_setFullAccessForTopic[channel] = false;
	}
	else if (arg_resp == "-t") {
		_setFullAccessForTopic[channel] = true;
	}
	std::string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() + "@localhost MODE " + channel + " :" + arg_resp + "\r\n";
	sendToClient(socketUser, response);
}

void Client::option_i(std::string channel, std::string option) {
	int socketUser = getClientSocket();
	std::string arg_resp;

	if (option == "+i")
		arg_resp = "+i";
	else
		arg_resp = "-i";

	if (arg_resp == "+i") {
		_setFullAccessInChannel[channel] = true;
	}
	else if (arg_resp == "-i") {
		_setFullAccessInChannel[channel] = false;
	}

	std::string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() + "@localhost MODE " + channel + " :" + arg_resp + "\r\n";
	sendToClient(socketUser, response);
}

void Client::capls() {
	return;
}

void Client::user() {
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string username = it->second;

	if (username.length() < 15) {
		if (_user.find(socketUser) != _user.end()){
			User checkUser = _user[socketUser];
			checkUser.setNickName(username);
			string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
			                  "@localhost " + "USER :" + username + "\r\n";


			sendToClient(socketUser, response);
			_user[socketUser] = checkUser;
		}
	}
	else {
		sendToClient(socketUser, "Username to long, max 8 characters\r\n");
	}
	_cmd.clear();
}


