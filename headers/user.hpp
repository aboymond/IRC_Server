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
	std::string								_nickName;
	std::string								_userName;
	int										_socket_user;
	bool									_moderator;
	bool									_userCreate;
	std::vector<std::string>				_channelName;
	std::map<std::string, bool>     		_isOperator;
	std::map<std::string, bool>				_userHaveGoodPassForEnterInChannel;
	std::map<std::string, bool>				_accessWithInvite;
	bool 									_who;
	bool									_PasswordIsValid;
	std::map<std::string, std::string> 		_channelTopic;
	std::vector<std::string>				_tmpBuffer;
	bool 									_itsOKToAddNick;
	bool 									_hasSetNick;


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
	void 				setIsOperator(const std::string &channel, bool isOperator);
	void 				setUserCreate(bool usercreate);
	void 				setChannelName(std::string channelname);
	void 				setPasswordIsValid( bool passWordIsValid );
	void 				setUserHaveGoodPassForEnterInChannel(std::string channel, bool passwordStatus);
	void 				setWho(bool who);
	void 				setAccessWithInvite(std::string channel, bool access);
	void 				setChannelandTopic(std::string channel, std::string topic);
	void				setTmpVectorBuffer(std::string &tmpBuffer);
	void				setItsOKToAddNick(bool istoktoaddnick);
	void 				setHasSetNick(bool hassetnick);



	// GET
	const std::string	&getNickName() const;
	const std::string	&getUserName() const;
	int					getSocketUser() const;
	bool				getOperator() const;
	bool 				&getIsOperator(std::string channel);
	bool 				getUserCreate() const;
	std::vector<std::string> getChannelName() const;
	bool 				searchChannel(std::string channelName) const;
	bool 				getPasswordIsValid() const;
	bool 				getWho() const;
	bool 				getUserHaveGoodPassForEnterInChannel(std::string channel);
	bool 				getAccessWithInvite(std::string channel);
	std::string 		getChannelTopic(std::string channel);
	bool				getItsOKToAddNick() const;
	bool 				getHasSetNick() const;



    // Init nick n username
  bool   				initUserAndNick();
	void 				delChannelName(std::string channel);
	void 				printAllChannel();
	void 				printChannelTopic();
	bool                userIsOnChannelWithTopic(std::string channel);
};

std::ostream &operator<<(std::ostream &o, User const &i);

#endif