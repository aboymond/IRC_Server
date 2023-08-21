#include "client.hpp"

Client::Client(){}

Client::Client(int socket) : _socket(socket), _isOperator(false) {}

Client::~Client() {}

int Client::getSocket() const {
    return(_socket);
}

const string &Client::getNickname() const {
    return(_nickName);
}

const string &Client::getUsername() const {
    return(_userName);
}

bool Client::isOperator() const {
    return(_isOperator);
}

void Client::setNickname(const string &nickname) {
    _nickName = nickname;
}

void Client::setUsername(const string &username) {
    _userName = username;
}

void Client::setOperator(bool isOperator) {
    _isOperator = isOperator;
}

void Client::parseNick_User(const string& data){
    istringstream iss(data);
    string line;
	Client client;

    while (getline(iss, line, '\n')){
//		printf("\tline = %s\n", line.c_str());
        if (line.find("NICK ") == 0){
			//write(1, "test", 4);
            string nick = line.substr(5);
			//nick.erase(remove(nick.begin(), nick.end(), '\n'), nick.end());
			//printf("\tnick = %s\n", nick.c_str());
			//_nickName = nick;
			for (int i = 0; nick[i]; i++){
				printf("char = %c, int = %i", nick[i] , nick[i]);
			}
			//client.setNickname(nick);
        }
        if (line.find("USER ") == 0){
            string user = line.substr(5);
            //_userName = user;
			client.setNickname(user);
        }
    }
    //cout << "Parse nick = " << client.getNickname() << " | user = " << client.getUsername() << endl;
}
