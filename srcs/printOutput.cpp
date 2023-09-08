#include "../headers/server.hpp"

void Server::serverPrintOutput(int port, int socketserver) {
	// Send in send.cpp = 1
	// Receive in server.cpp = 2

	cout << "\033[1;34m----- START SERVER -----\033[0m" << endl;

	cout << "IP [ \033[1;34m" << IP_SERV << "\033[0m ]" << endl;
	cout << "PORT [ \033[1;34m" << port << "\033[0m ]" << endl;
	cout << "PASSWORD [ \033[1;34m" << _password << "\033[0m ]" << endl;
	cout << "SOCKET SERVER [ \033[1;34m" << socketserver << "\033[0m ]" << endl;

	cout << "\033[1;34m----------------------------\033[0m\n" << endl;

}

void Client::printOutput(int numofoption, string message, int options, int fd) {
	// Send in send.cpp = 1
	// Receive in server.cpp = 2
	(void) options;
	if (_user.find(fd) != _user.end()){
		const User &checkUser = _user[fd];
		if (numofoption < 1) {
			cout << "Error print output" << endl;
			return;
		}
		if (numofoption == 1) {
			cout << "\033[1;93m----- CLIENT TO SERVER ------\033[0m" << endl;

			cout << "FROM CLIENT [ \033[1;93m" << checkUser.getSocketUser() << "\033[0m ] TO SERVER" << endl;
			cout << "NICKNAME [ \033[1;93m" << checkUser.getNickName() << "\033[0m ]" << endl;
			cout << "USERNAME [ \033[1;93m" << checkUser.getUserName() << "\033[0m ]" << endl;
			cout << "PASSWORD [ \033[1;93m" << checkUser.getPasswordIsValid() << "\033[0m ]" << endl;
			cout << "<<<<  \033[1;93m" << message << "\033[0m";

			cout << "\033[1;93m----------------------------\033[0m\n" << endl;
		}
		else if (numofoption == 2) {
			cout << "\033[1;96m----- SERVER TO CLIENT -----" << endl;

			cout << "FROM SERVER TO CLIENT [ \033[1;96m" << checkUser.getSocketUser() << "\033[0m ]" << endl;
			cout << "NICKNAME [ \033[1;96m" << checkUser.getNickName() << "\033[0m ]" << endl;
			cout << "USERNAME [ \033[1;96m" << checkUser.getUserName() << "\033[0m ]" << endl;
			cout << ">>>> \033[1;96m" << message << "\033[0m";

			cout << "\033[1;96m----------------------------\033[0m\n" << endl;
		}
		else if (numofoption == 3) {
			cout << "\033[1;91----- DECONNECTION -----" << endl;

			cout << "CLIENT [ \033[1;91" << checkUser.getSocketUser() << "\033[0m ]" << endl;
			cout << "NICKNAME [ \033[1;91" << checkUser.getNickName() << "\033[0m ]" << endl;
			cout << "USERNAME [ \033[1;91" << checkUser.getUserName() << "\033[0m ]" << endl;

			cout << "\033[1;91----------------------------\033[0m\n" << endl;
		}
	}
}
