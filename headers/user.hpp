#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <cstdio>
#include "client.hpp"
#include <map>


class User {

protected:
	//Client		*_client;

	std::string						_nickName;
	std::string						_userName;
	int								_socket_user;
	bool							_moderator;
	bool							_userCreate;
	std::vector<std::string>		_channelName;
	std::map<std::string, bool>        _isOperator;

	bool 							_who;

	bool							_PasswordIsValid;


public:

	User();
	User(User const &src);
	User &operator=(User const &rhs);
	~User();

	// SET
	void				setNickName(const std::string &nickname);
	void				setUserName(const std::string &username);
	void				setSocketUser(int socket_user);
	void				setOperator(bool moderator);
	void 				setIsOperator(std::string channel, bool isOperator);
	void 				setUserCreate(bool usercreate);
	void 				setChannelName(std::string channelname);
	void 				setPasswordIsValid( bool passWordIsValid );
	void 				setWho(bool who);
	void         		setIsOperator(std::string channel, bool isOperator);
	bool         		getIsOperator(std::string channel);


	// GET
	const std::string	&getNickName() const;
	const std::string	&getUserName() const;
	int					getSocketUser() const;
	bool				getOperator() const;
	bool 				getIsOperator(std::string channel) ;
	bool 				getUserCreate() const;
	std::vector<std::string> getChannelName() const;
	bool 				searchChannel(std::string channelName) const;
	bool 				getPasswordIsValid() const;
	bool 				getWho() const;


	void 				printAllChannel();
	void 				delChannelName(std::string channel);


    // Init nick n username
    bool   				initUserAndNick(std::string buffer);

	void 				delChannelName(std::string channel);

	void 				printAllChannel();

};

std::ostream &operator<<(std::ostream &o, User const &i);

#endif