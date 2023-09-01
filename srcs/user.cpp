#include "../headers/user.hpp"

User::User() :
	//_client(),
	_nickName(""),
	_userName(""),
	_socket_user(0),
	_moderator(false),
	_userCreate(false)
{
	cout << "constructor client called" << endl;
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

void 		User::setUserCreate(bool usercreate){
	_userCreate = usercreate;
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

bool 		User::getUserCreate() const{
	return (_userCreate);
}

bool    User::initUserAndNick(string buffer){
    std::string nickName;
    std::string userName;
    istringstream iss(buffer);
    string line;
    size_t i = 5;
    while (std::getline(iss, line, '\n')) {
		if (line.find("CAP LS") == 0)
			i = 13;
        if (line.find("NICK ") == 0) {
            while ( i < buffer.length() && buffer[i] != '\r') {
                nickName.push_back(buffer[i]);
                i++;
            }
            setNickName(nickName);
        }
        if (line.find("USER ") == 0) {
            i += 7;
            while (i < buffer.length() && buffer[i] != ' ' && buffer[i] != '\r') {
                userName.push_back(buffer[i]);
                i++;
            }
            setUserName(userName);

        }
    }
	cout << "nick in init = " << nickName << endl;
	if (nickName.empty())
		return (false);
	return (true);
}