#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <sstream>

using namespace std;

class Client {

private:
    int         _socket;
    string      _nickName;
    string      _userName;
    // Pour que seul les clients qui crees un channel puissent avoir les droits
    bool        _isOperator;
	bool		_nick_user_init;

public:
    Client(int socket);
	Client();
    ~Client();

    //Get infos client
    int     getSocket() const;
    const string &getNickname() const;
    const string &getUsername() const;
    bool    isOperator() const;
	bool 	getNickUserInit() const;

    //Set infos client
	void setSocket(const int socket);
    void setNickname(const string &nickname);
    void setUsername(const string &username);
    void setOperator(const bool isOperator);
	void setNickUserInit(const bool nick_user_init);

    //void    initUser();
    void    parseNick_User(const string& data);
};

#endif