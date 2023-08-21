#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Client {

private:
    int         _socket;
    string      _nickName;
    string      _userName;
    // Pour que seul les clients qui crees un channel puissent avoir les droits
    bool        _isOperator;

public:
    Client(int socket);
    ~Client();

    //Get infos client
    int     getSocket() const;
    const string &getNickname() const;
    const string &getUsername() const;
    bool    isOperator() const;

    //Set infos client
    void setNickname(const string &nickname);
    void setUsername(const string &username);
    void setOperator(const bool isOperator);

    //void    initUser();
    void    parseNick_User(const string& data);
};

#endif