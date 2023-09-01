#include "../headers/client.hpp"

void Client::parsCommands(string buffer, int socketUser){
	(void)socketUser;
//	cout << "UPPER AVANT = " << buffer << endl;
//	transform(buffer.begin(), buffer.end(), buffer.begin(), ::toupper);
//	cout << "UPPER APRES = " << buffer << endl;
	if (strncmp(buffer.c_str(), "NICK ", 5) == 0 ){
		if (buffer.length() < 25){
			size_t space = buffer.find(' ');
			if (space < 5) {
				string command = buffer.substr(0, 4);
				string nickname = buffer.substr(space + 1, buffer.length() - space - 3);
				_cmd[command] = nickname;
				nick(socketUser);
			}
		}
		else
			sendToClient(socketUser, "Beaucoup trop long, comme ma b*** !\r\n");
	}
	else if (strncmp(buffer.c_str(), "JOIN #", 6) == 0){
		cout << "COUCOU" << endl;
	}
	else
		cout << "Buffer de parse = " << buffer << endl;
}

//void Client::commandToFunction(string buffer, int socketUser){
//	(void)socketUser;
//	string cmd[1] = {"JOIN"};
//	void (Client::*ptr_cmd[1]) (void) = {&Client::join};
//	for (int i = 0; i < 4; i++){
//		if (cmd[i] == buffer) (this->*ptr_cmd[i])();
//
//	}
//}

//void Client::join(){
//	cout << "DANS JOIN" << endl;
//}
//
void Client::nick(int socketUser){
	if (_user.find(socketUser) != _user.end()){
			std::map<string, string>::iterator it;
			it = _cmd.begin();
			cout << "it = " << it->first << "it sec = " << it->second << endl;
		User checkUser = _user[socketUser];
		checkUser.setNickName(_cmd[NICK]);
		string response = ":" + _user[socketUser].getNickName() + "!~" + _user[socketUser].getUserName() +
						  "@localhost " + "NICK :" + it->second + "\r\n";


		sendToClient(socketUser, response);
		_user[socketUser] = checkUser;
	}
}