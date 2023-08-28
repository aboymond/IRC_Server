#include "user.hpp"

User::User() :
	//_client(),
	_nickName(""),
	_userName(""),
	_socket_user(0),
	_moderator(false)
{

};

User::~User() {};

User::User(User const &src){
	*this = src;
}
User &User::operator=(User const &rhs){
	if (this != &rhs){
		_nickName = 	rhs._nickName;
		_userName = 	rhs._userName;
		_socket_user = 	rhs._socket_user;
		_moderator = 	rhs._moderator;
	}
	return (*this);
}
// SET

void		User::setNickName(const std::string &nickname){
	_nickName = nickname;
}

void		User::setUserName(const std::string &username){
	_userName = username;
}

void		User::setSocketUser(int socket_user){
	_socket_user = socket_user;
}

void		User::setOperator(bool moderator){
	_moderator = moderator;
}

// GET

const std::string &User::getNickName() const{
	return (_nickName);
}

const std::string &User::getUserName() const{
	return (_userName);
}

int			User::getSocketUser() const{
	return (_socket_user);
}

bool		User::getOperator() const{
	return (_moderator);
}