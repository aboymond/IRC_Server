#include "../headers/user.hpp"

User::User() :
	_nickName(""),
	_userName(""),
	_socket_user(0),
	_moderator(false),
	_userCreate(false),
	_who(false),
	_PasswordIsValid(false)
{
	cout << "constructor user called" << endl;
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
		_channelName = rhs._channelName;
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

void 		User::setIsOperator(std::string channel, bool isOperator){
	_isOperator.insert(std::make_pair(channel, isOperator));
}

void 		User::setUserCreate(bool usercreate){
	_userCreate = usercreate;
}

void 		User::setChannelName(std::string channelname){
	_channelName.push_back(channelname);
}

void 		User::setPasswordIsValid(bool passWordIsValid) {
	_PasswordIsValid = passWordIsValid;
}

void 		User::setWho(bool who) {
	_who = who;
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

bool 		User::getIsOperator(std::string channel) {
	bool tmp_isOP = false;
	if(_isOperator.find(channel) != _isOperator.end()){
		tmp_isOP = _isOperator[channel];

	}
	return (tmp_isOP);
}

bool 		User::getUserCreate() const{
	return (_userCreate);
}


bool 		User::getWho() const {
	return (_who);
}

vector<string>	User::getChannelName() const{
	return (_channelName);
}


bool User::searchChannel(std::string channelName) const {
	for (std::vector<std::string>::const_iterator it = _channelName.begin(); it != _channelName.end(); ++it){
		cout << "it = " << *it << endl;
		if (it->compare(channelName) == 0){
			cout << "true" << endl;
			return true;
		}
	}
	return false;
}


bool 		User::getPasswordIsValid() const{
	return (_PasswordIsValid);

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
	if (nickName.empty())
		return (false);
	return (true);
}

void User::printAllChannel() {
	for (std::vector<std::string>::const_iterator it = _channelName.begin(); it != _channelName.end(); ++it){
		cout << "Is on channel : " << *it << endl;
	}
}

bool         User::getIsOperator(std::string channel) {
	bool tmp_isOP = false;
	if(_isOperator.find(channel) != _isOperator.end()){
		tmp_isOP = _isOperator[channel];
	}
	return (tmp_isOP);
}

void         User::setIsOperator(std::string channel, bool isOperator){
	_isOperator.insert(std::make_pair(channel, isOperator));
}

void 		User::delChannelName(std::string channel) {
	for (vector<std::string>::iterator it = _channelName.begin(); it != _channelName.end(); it++) {
		cout << "it = " << *it << endl;
		if (*it == channel)
		{
			_channelName.erase(it);
			break ;
		}
  }
}

std::ostream &operator<<(std::ostream &o, User const &i) {
	o << "Username " << i.getUserName() << "\n"
		 "Moderator = : " << i.getOperator() << "\n" << endl;
	return (o);
}


