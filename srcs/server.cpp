#include "../headers/server.hpp"

Server::Server() :
	_socketServer(0),
	_port(0),
	_password("Default"),
	_validPassword(false) {

};

Server::Server(int port, string password) : _port(port), _validPassword(false) {
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	setPassword(password);
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

void Server::setPassword(std::string password) {
	_password = password;
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
	socklen_t addrlen = sizeof(_serverAddress);
	fd_set readfds;
	Client client;
	std::map<int, std::string> clientBuffers;

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
			client.sendToClient(tmp_user_socket, "Enter the password with /PASS\r\n");
			client.setServerPassword(_password);
		}

		for (size_t i = 0; i < _userSocket.size(); i++) {
			int sd = _userSocket[i];
			char buffer[1024];
			memset(buffer, '\0', strlen(buffer));
			if (FD_ISSET(sd, &readfds)) {
				size_t val_read;
				if ((val_read = recv(sd, buffer, 1024, 0)) == 0) {
					cout << "rentre dans le if" <<endl;
					client.eraseUser(sd);
					close(sd);
					_userSocket.erase(_userSocket.begin() + (int)i);
				} else {
					cout << "rentre dans le else" <<endl;
					buffer[val_read] = '\0';

					client.printOutput(1, buffer, 0, sd);
					size_t pos;
					clientBuffers[sd] += buffer;
					while ((pos = clientBuffers[sd].find("\r\n")) != std::string::npos || (pos = clientBuffers[sd].find("\n")) != std::string::npos) {
						std::string fullMessage = clientBuffers[sd].substr(0, pos);
						client.printOutput(1, fullMessage, 0, sd);

						cout << "buffer passe par la = " << fullMessage << endl;
//						if (client.addUser(buffer, sd)) {
//							if (client.passwordVerifier(sd) == false) {
								if (fullMessage.find("QUIT") != string::npos || fullMessage.find("quit") != string::npos ) {
									client.quit();
									close(sd);
									_userSocket.erase(_userSocket.begin() + (int) i);
								}
								else
								{
									client.addUser(fullMessage, sd);
									client.setClientSocket(sd);
									client.parsCommands(fullMessage);
									client.checkAndExecuteCmd();
								}
//								client.pass();
//							}
//							else {
//								client.setClientSocket(sd);
//								client.parsCommands(buffer);
//								client.checkAndExecuteCmd();
//								if (strncmp(buffer, "QUIT ", 5) == 0) {
//									client.quit();
//									close(sd);
//									_userSocket.erase(_userSocket.begin() + (int) i);
//								}
//							}
//						}
						clientBuffers[sd].erase(0, pos + 2);
					}
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
