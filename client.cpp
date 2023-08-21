#include "client.hpp"

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

    while (getline(iss, line, '\n')){
        if (line.find("NICK ") == 0){
            string nick = line.substr(5);
            _nickName = nick;
        }
        if (line.find("USER ") == 0){
            string user = line.substr(5);
            _userName = user;
        }
    }
    cout << "Parse nick = " << _nickName << " | user = " << _userName << endl;
}
