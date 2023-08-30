#include "../headers/client.hpp"

void Client::sendToClient(int fd, std::string message) {
	if (message.length() <= 0)
		cout << "Chelou ca ne devrait pas arriver ! Check sendToClient dans send.cpp" << endl;
	else {
		printOutput(2, message, 0, fd);
		send(fd, message.c_str(), message.length(), 0);
	}

}