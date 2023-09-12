#include "../headers/client.hpp"

void Client::parsCommands(string buffer) {

	std::stringstream ss(buffer);
	std::string command;
	std::string argument;
	ss >> command;
	std::getline(ss, argument);

	argument.erase(argument.length() - 1);
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


	std::string	cmd[NBR_OF_CMD] = { "NICK", "JOIN", "WHO", "KICK", "PRIVMSG", "PASS", "PART", "TOPIC", "MODE" };
	void (Client::*ptr_command[NBR_OF_CMD]) (void) = { &Client::nick, &Client::join, &Client::who, &Client::kick,
											  &Client::privmsg, &Client::pass, &Client::part, &Client::topic, &Client::mode };
	for (int i = 0; i < NBR_OF_CMD; i++) {

		if (cmd[i] == command) {
			(this->*ptr_command[i])();
			commandFound = true;
		}
	}
	if (commandFound == false)
		sendToClient(getClientSocket(), "Command not found: " + command + "\r\n");
	_cmd.clear();
}

void Client::join(){
	int clientSocket = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string channel = it->second;

	if (_user.find(clientSocket) != _user.end()){
		std::map<string, string>::iterator it;
		it = _cmd.begin();
		if (checkChannelExist(it->second) != true){

			_user[clientSocket].setChannelName(channel);
			_user[clientSocket].setOperator(true);
			_user[clientSocket].setIsOperator(channel, true);
			setWhoIsOP(channel, _user[clientSocket].getNickName());
			_user[clientSocket].setWho(false);
			cout << _user[clientSocket].getNickName() << " is operator !" << endl;
		}
		else {
			_user[clientSocket].setChannelName(channel);
			_user[clientSocket].setIsOperator(channel, false);
			_user[clientSocket].setWho(false);
		}
		for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
			User currentUser = it->second;
			string responseToJoin = ":" + _user[clientSocket].getNickName() + "!~" + _user[clientSocket].getUserName() +
							  "@localhost " + "JOIN " + ":" + channel + "\r\n";
			sendToClient(currentUser.getSocketUser(), responseToJoin);

		}
	}
//	< :*.freenode.net 352 lois #test42 ~boris freenode-o6d.g28.dc9e5h.IP *.freenode.net jean H :0 Alexandre Boymond
//	< :*.freenode.net 352 lois #test42 ~aboymond freenode-o6d.g28.dc9e5h.IP *.freenode.net piow00 H@ :0 Alexandre Boymond
//	< :*.freenode.net 352 lois #test42 ~bichou freenode-o6d.g28.dc9e5h.IP *.freenode.net lois H :0 Alexandre Boymond
	_user[clientSocket].printAllChannel();
	_cmd.clear();
}

void Client::nick(){
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string nickname = it->second;

	if (nickname.length() < 8) {
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

				resp_who = ":*." + (string) IP_SERV + " 354 " + _user[socketUser].getNickName() + " 152 " + channel +
						   " " + currentUser.getNickName() + " :H" + op + "\r\n";
				sendToClient(socketUser, resp_who);
				resp_who.erase();
			}
		}
		resp_who =  ":*." + (string) IP_SERV + " 315 " + _user[socketUser].getNickName() + " " + channel + " :End of /WHO list.\r\n";
		sendToClient(socketUser, resp_who);
		resp_who.erase();
		op.erase();
	}
	else {
		if (_user[socketUser].getIsOperator(channel) == true)
			op = "@";
		else
			op = "@" + _whoIsOP[channel] + " ";

		resp_who = ":*." + (string) IP_SERV + " 353 " + _user[socketUser].getNickName() + " = " + channel +
				   " :" + op + _user[socketUser].getNickName() + "\r\n"
				  	":*." + (string)IP_SERV + " 352 " + _user[socketUser].getNickName() + " " + channel + " ~" + _user[socketUser].getUserName() + " " + (string)IP_SERV + " *." + (string)IP_SERV + " " + _user[socketUser].getNickName() + " H :0\r\n"
				 	":*." + (string) IP_SERV + " 315 " + _user[socketUser].getNickName() +
					 " " + channel + " :End of /WHO list.\r\n";
		sendToClient(socketUser, resp_who);
		resp_who.erase();
		op.erase();
		_user[socketUser].setWho(true);
	}
	_cmd.clear();
}

//void	Client::who() {
//	int socketUser = getClientSocket();
//	std::map<std::string, std::string>::iterator it = _cmd.begin();
//	std::string channel = it->second;
//
//
//
//	_cmd.clear();
//}

void    Client::kick(){
	int socketUser = getClientSocket();
	map<std::string, std::string>::iterator it_chan = _cmd.begin();
	map<std::string, std::string>::iterator it_argument = _cmd.begin();
	map<int, User>::iterator it;
	vector<string>::iterator it_channel;

	string channel = extractChannelName(it_chan->second);
	string userToKick = it_argument->second;
	string commandAndChannel = "KICK " + channel;
	size_t found = userToKick.find_last_of(' ');
	string user = it_argument->second.substr(found + 1, it_argument->second.length() - found);

	string response = ":" + user + "!~" + user + "@localhost " + commandAndChannel + " " + user + " :" + user + "\r\n";
//	sendToClient(socketUser, response);
	if (_user[socketUser].getOperator() == true)
	{
		if (checkChannelExist(channel) == true)
		{
			for (it = _user.begin(); it != _user.end(); it++)
			{
				User &currentUser = it->second;
				for (it_channel = currentUser.getChannelName().begin(); it_channel < currentUser.getChannelName().end(); ++it_channel) {
					if (*it_channel == channel) {
						if (currentUser.getNickName() != user) {
							sendToClient(socketUser, response);
						}
						else
							sendToClient(socketUser, ":" + (string)IP_SERV + " 401 " + _user[socketUser].getNickName() + " " + user + " :No such Nick\r\n" );
					}
					else
						sendToClient(socketUser, "Channel doesn't exist\r\n");
				}
			}
		}
		else
			sendToClient(socketUser, "Channel doesn't exist\r\n");
	}
	_cmd.clear();
}

void    Client::part(){
	int socketUser = getClientSocket();
//	map<std::string, std::string>::iterator it_chan = _cmd.begin();
	map<std::string, std::string>::iterator it_argument = _cmd.begin();
	map<int, User>::iterator it;
	vector<string>::iterator it_channel;
	size_t 					space;

//	string channel = extractChannelName(it_chan->second);
//	cout << "channel = " << channel << endl;
	string channelToleave = it_argument->second;
	space = channelToleave.find(' ');
	string commandAndChannel = "PART " + channelToleave;
	cout << "channelToLeave = " << channelToleave << endl;
	string user = _user[socketUser].getUserName();
	string channel = channelToleave.substr(0, space);

//	cout << "response = " << response << endl;
	string response = ":" + user + "!~" + user + "@localhost" + " PART " + channel + " :\"Leaving\"\r\n";
//	:USER2!~USER2@freenode-o6d.g28.dc9e5h.IP PART #test43 :"Leaving"
		if (checkChannelExist(channel) == true)
		{
			for (map<int, User>::iterator it = _user.begin(); it != _user.end() ; ++it) {
				cout << "username = " << it->second.getUserName() << endl;
				User currentUser = it->second;
				if (currentUser.getSocketUser() != socketUser)
				{
					string response2 = ":" + _user[socketUser].getUserName() + "!~" + "@localhost" + " PART " + channel + " :\"Leaving\"\r\n";
					sendToClient(currentUser.getSocketUser(), response2);
				}
			}
			sendToClient(socketUser, response);
			_user[socketUser].delChannelName(channel);
		}
		else
			sendToClient(socketUser, "Channel doesn't exist\r\n");

	_user[socketUser].printAllChannel();
	_cmd.clear();
}

void    Client::topic(){
	int socketUser = getClientSocket();
	map<std::string, std::string>::iterator it_argument = _cmd.begin();
	string argument = it_argument->second;
	string user = _user[socketUser].getUserName();
	string channel = argument.substr(0, argument.find(' '));
	string nameOfChannelTopic = argument.substr(argument.find(':'), argument.length());
//	:*.freenode.net 482 USER2 #test42 :You do not have access to change the topic on this channel
	string responseIfchannelNotExiste = ":*.@localhost 403 " + user + " " + nameOfChannelTopic + " :No such channel\r\n";
	string responseIfChannelCanHaveTop = ":" + user + "!~" + user + "@localhost" + " TOPIC " + argument + "\r\n";
	string responseIfUserHaveNoPermission = ":*.localhost 482 " + user + " " + channel + " :You do not have access to change the topic on this channel\r\n";
	if (checkChannelExist(channel) == true)
	{
		if (_user[socketUser].getIsOperator(channel) == true)
		{
			sendToClient(socketUser, responseIfChannelCanHaveTop);

		}
		else
		{
			sendToClient(socketUser, responseIfUserHaveNoPermission);
		}
	}
	else
		sendToClient(socketUser, responseIfchannelNotExiste);
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

void Client::mode() {
	int socketUser = getClientSocket();
	std::map<std::string, std::string>::iterator it = _cmd.begin();
	std::string channel = it->second;
	vector<string>::iterator it_channel;
	std::string arg;
	std::string nickName;
	std::stringstream ss(channel);
	ss >> channel >> arg;
	if (!arg.empty()) {
		ss >> nickName;
		nickName.erase(nickName.length());
	}

	std::string arg_resp;

	if (_user[socketUser].getIsOperator(channel) == true){
		if (arg == "-o" || arg == "+o") {
			if (arg == "+o")
				arg_resp = "+o";
			else
				arg_resp = "-o";
			if (UserIsOnChannel(nickName, channel) == true){

				for (std::map<int, User>::iterator it = _user.begin(); it != _user.end(); ++it) {
					User currentUser = it->second;
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
				cout << "TEEEEEEEEEEEEEESSSSSSSST IN MOOOOODE" << endl;
			}
			cout << "In mode channel = " << channel << " | arg = " << arg << " | nickname = " << nickName << endl;
		}
		cout << "Bad argument in MODE" << endl;
	}
	else {
		cout << "Argument not found" << endl;
	}


	// i: t: k: o:

	// o : :piow00!~aboymond@freenode-o6d.g28.dc9e5h.IP MODE #test42 +o :jean

	//if (_user[socketUser].)
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
