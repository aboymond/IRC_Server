#include "client.hpp"

#include <stdio.h>
#include <unistd.h>

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

    while (getline(iss, line, '\n')){
        size_t compt = 0;
        if (line.find("NICK ") == 0){
            compt = 5;
            while (compt < line.length() && line[compt] != '\r'){
                compt++;
            }
            string nick = line.substr(5, compt - 5);
            setNickname(nick);
        }

        if (line.find("USER ") == 0){
            compt = 5;
            while (compt < line.length() && line[compt] != ' ')
                compt++;
            string user = line.substr(5, compt - 5);
            setUsername(user);
        }
    }
}
