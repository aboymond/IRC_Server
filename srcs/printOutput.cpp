#include "../headers/server.hpp"
#include "../headers/client.hpp"

void Server::serverPrintOutput(int port, int socketserver) {
	// Send in send.cpp = 1
	// Receive in server.cpp = 2

	cout << "\033[1;34m----- START SERVER -----\033[0m" << endl;

	cout << "IP [ \033[1;34m" << IP_SERV << "\033[0m ]" << endl;
	cout << "PORT [ \033[1;34m" << port << "\033[0m ]" << endl;
	cout << "SOCKET SERVER [ \033[1;34m" << socketserver << "\033[0m ]" << endl;

	cout << "\033[1;34m----------------------------\033[0m\n" << endl;

}

void Client::PrintOutput(int numofoption, string message, int options, int fd) {
	// Send in send.cpp = 1
	// Receive in server.cpp = 2
	(void)options;
	if (numofoption < 1) {
		cout << "Error print output" << endl;
		return;
	} if (numofoption == 1) {
		for (size_t i = 0; i < _user.size(); i++) {
			const User &checkUser = _user[i];
			if (checkUser.getSocketUser() == fd) {
				cout << "\033[1;31m----- CLIENT TO SERVER ------\033[0m" << endl;

				cout << "FROM CLIENT [ \033[1;31m" << checkUser.getSocketUser() << "\033[0m ] TO SERVER" << endl;
				cout << "NICKNAME [ \033[1;31m" << checkUser.getNickName() << "\033[0m ]" << endl;
				cout << "USERNAME [ \033[1;31m" << checkUser.getUserName() << "\033[0m ]" << endl;
				cout << "<<<<  \033[1;31m" << message << "\033[0m";

				cout << "\033[1;31m----------------------------\033[0m\n" << endl;
			}
		}
	} else if (numofoption == 2) {
		for (size_t i = 0; i < _user.size(); i++) {
			const User &checkUser = _user[i];
			if (checkUser.getSocketUser() == fd) {
				cout << "\033[1;32m----- SERVER TO CLIENT -----" << endl;

				cout << "FROM SERVER TO CLIENT [ \033[1;32m" << checkUser.getSocketUser() << "\033[0m ]" << endl;
				cout << "NICKNAME [ \033[1;32m" << checkUser.getNickName() << "\033[0m ]" << endl;
				cout << "USERNAME [ \033[1;32m" << checkUser.getUserName() << "\033[0m ]" << endl;
				cout << ">>>> \033[1;32m" << message << "\033[0m";

				cout << "\033[1;32m----------------------------\033[0m\n" << endl;
			}
		}
	}
}