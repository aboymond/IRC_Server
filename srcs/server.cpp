#include "../headers/server.hpp"

Server::Server() :
		_socketServer(0),
		_port(0),
		_password("Default"),
		_validPassword(false) {

};

Server::Server(int port, string password) : _port(port), _password(password), _validPassword(false) {
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketServer < 0)
		cout << "Error socket server" << endl;
	int optionFlag = 1;
	if (setsockopt(_socketServer, SOL_SOCKET, SO_REUSEADDR, &optionFlag, sizeof(optionFlag))) {
		cout << "Error setsockopt" << endl;
	}
	fcntl(_socketServer, F_SETFL, O_NONBLOCK);
};

Server::Server(const Server &src) {
	*this = src;
}

Server::~Server() {};

Server &Server::operator=(const Server &rhs) {
	if (this != &rhs) {
		this->_socketServer = rhs._socketServer;
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->_validPassword = rhs._validPassword;
	}
	return (*this);
}

int Server::getPort() const {
	return (this->_port);
}

int Server::getSocketServer() const {
	return (this->_socketServer);
}

string Server::getPassword() const {
	return (this->_password);
}

bool Server::getValidPassword() const {
	return (this->_validPassword);
}

const vector<int>& Server::getUserSockets() const {
	return _userSocket;
}

//const Client* Server::getClient() const {
//	return _client;
//}

void Server::createSocketServer() {


	_serverAddress.sin_addr.s_addr = inet_addr(IP_SERV);
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_port = htons(_port);

	bind(_socketServer, (struct sockaddr *) &_serverAddress, sizeof(_serverAddress));
	listen(_socketServer, 5);
	serverPrintOutput(_port, _socketServer);

}

void Server::waitToNewConnection() {

	int max_fd = _socketServer;
	string tabCommand[] = {"NICK ", "JOIN "};
	socklen_t addrlen = sizeof(_serverAddress);
	fd_set readfds;
	Client client;

	while (true) {
		FD_ZERO(&readfds);
		FD_SET(_socketServer, &readfds);

		for (size_t i = 0; i < _userSocket.size(); i++) {
			int fd_tmp = _userSocket[i];
			if (fd_tmp > 0)
				FD_SET(fd_tmp, &readfds);
			if (fd_tmp > max_fd)
				max_fd = fd_tmp;
		}

		int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);

		if (activity < 0 && errno != EINTR)
			cout << "error" << endl;

		if (FD_ISSET(_socketServer, &readfds)) {
			int tmp_user_socket;
			if ((tmp_user_socket = accept(_socketServer, (struct sockaddr *) &_serverAddress, &addrlen)) < 0) {
				cout << "Error with accept" << endl;
			}

			fcntl(tmp_user_socket, F_SETFL, O_NONBLOCK);
			_userSocket.push_back(tmp_user_socket);
		}

		for (size_t i = 0; i < _userSocket.size(); i++) {
			int sd = _userSocket[i];
			char buffer[1024];
			memset(buffer, '\0', strlen(buffer));
			if (FD_ISSET(sd, &readfds)) {
				size_t val_read;
				if ((val_read = recv(sd, buffer, 1024, 0)) == 0) {
					//client.eraseUser(sd);
					//client.printOutput(3, NULL, 0, sd);
					client.eraseUser(sd);
					close(sd);
					_userSocket.erase(_userSocket.begin() + (int)i);
				} else {
					buffer[val_read] = '\0';
					if(client.addUser(buffer, _userSocket[i])){

						client.parsCommands(buffer, _userSocket[i]);
						break;
					}

					client.printOutput(1, buffer, 0, _userSocket[i]);
				}
			}

		}


	}
	close(_socketServer);
}

const char *Server::BadArgument::what() const throw() {
	return ("Error: need to have 3 arguments: ./ircserv <port> <password>");
}

const char *Server::FailOpeningSocket::what() const throw() {
	return ("Error: impossible to opening socket");
}

std::ostream &operator<<(std::ostream &o, Server const &i) {
	o <<
	  "Socket server Number: " << i.getSocketServer() << "\n"
	                                                     "Connect to port: " << i.getPort() << "\n"
	                                                                                           "Password: "
	  << i.getPassword() << "\n"
	                        "Password status: " << i.getValidPassword() << "\n";
	return (o);
}