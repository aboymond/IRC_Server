#include "../headers/user.hpp"

User::User() :
	_nickName(""),
	_userName(""),
	_socket_user(0),
	_moderator(false),
	_userCreate(false),
	_who(false),
	_PasswordIsValid(false),
	_hasSetNick(false)
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

void 		User::setIsOperator(const std::string &channel, bool isOperator){

	if (_isOperator.find(channel) != _isOperator.end()) {
		_isOperator[channel] = isOperator;
	} else {
		_isOperator.insert(make_pair(channel, isOperator));
	}

}

void 		User::setUserHaveGoodPassForEnterInChannel(std::string channel, bool passwordStatus) {
	if (_userHaveGoodPassForEnterInChannel.find(channel) != _userHaveGoodPassForEnterInChannel.end()) {
		_userHaveGoodPassForEnterInChannel[channel] = passwordStatus;
	} else {
		_userHaveGoodPassForEnterInChannel.insert(make_pair(channel, passwordStatus));
	}
}
void 		User::setAccessWithInvite(std::string channel, bool access) {
	if (_accessWithInvite.find(channel) != _accessWithInvite.end()) {
		_accessWithInvite[channel] = access;
	} else {
		_accessWithInvite.insert(make_pair(channel, access));
	}
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

bool 		&User::getIsOperator(std::string channel) {
	return (_isOperator[channel]);
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

bool 			User::getUserHaveGoodPassForEnterInChannel(std::string channel) {
	return (_userHaveGoodPassForEnterInChannel[channel]);
}

bool 			User::getAccessWithInvite(std::string channel) {
	return (_accessWithInvite[channel]);
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

bool    User::initUserAndNick(){
    std::string nickName;
    std::string userName;

    for (size_t i = 0; i < _tmpBuffer.size(); i++) {
		if (_tmpBuffer[i].find("CAP LS") == 0)
			i++;
        if (_tmpBuffer[i].find("NICK ") == 0) {
			stringstream ss1(_tmpBuffer[i]);
			string cmd;
			ss1 >> cmd >> nickName;
			cout << "NICK IN INIT : " << nickName << endl;
            setNickName(nickName);
        }
        if (_tmpBuffer[i].find("USER ") == 0) {
			stringstream ss2(_tmpBuffer[i]);
			string cmd2;
			ss2 >> cmd2 >> userName;
			cout << "USER IN INIT : " << userName << endl;
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

void	User::setChannelandTopic(std::string channel, std::string topic) {
	_channelTopic.insert(std::make_pair(channel, topic));
}

void User::printChannelTopic() {
	std::map<string, string>::iterator it;
	for (it = _channelTopic.begin(); it != _channelTopic.end(); ++it) {
		cout << "channel = " << it->first << endl;
		cout << "topic = " << it->second << endl;
	}
}

bool User::userIsOnChannelWithTopic(string channel) {
	std::map<string, string>::iterator it;

	for (it = _channelTopic.begin(); it != _channelTopic.end(); ++it) {
		if (it->first == channel)
			return true;
	}
	return (false);
}

string User::getChannelTopic(string channel) {
		return (_channelTopic[channel]);
}



std::ostream &operator<<(std::ostream &o, User const &i) {
	o << "Username " << i.getUserName() << "\n"
		 "Moderator = : " << i.getOperator() << "\n" << endl;
	return (o);
}

void		User::setTmpVectorBuffer(std::string &tmpBuffer) {
	_tmpBuffer.push_back(tmpBuffer);
}

void				User::setItsOKToAddNick(bool istoktoaddnick) {
	_itsOKToAddNick = istoktoaddnick;
}

bool				User::getItsOKToAddNick() const {
	return (_itsOKToAddNick);
}

void				User::setHasSetNick(bool hassetnick) {
	_hasSetNick = hassetnick;
}

bool				User::getHasSetNick() const {
	return (_hasSetNick);
}


