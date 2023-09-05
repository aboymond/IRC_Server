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


class User {

protected:
	//Client		*_client;

	std::string						_nickName;
	std::string						_userName;
	int								_socket_user;
	bool							_moderator;
	bool							_userCreate;
	std::vector<std::string>		_channelName;

	bool		_PasswordIsValid;


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
	void 				setUserCreate(bool usercreate);
	void 				setChannelName(std::string channelname);
	void 				setPasswordIsValid( bool passWordIsValid );

	// GET
	const std::string	&getNickName() const;
	const std::string	&getUserName() const;
	int					getSocketUser() const;
	bool				getOperator() const;
	bool 				getUserCreate() const;
	bool 				searchChannel(std::string channelName) const;
	bool 				getPasswordIsValid() const;

    // Init nick n username
    bool   initUserAndNick(std::string buffer);
};

#endif