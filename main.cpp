#include "server.hpp"

int main( int argc, char** argv ) {

	try
	{
		if (argc != 3) {
			throw Server::BadArgument();
		}
		else
		{
			int port = atoi(argv[1]);
			std::string password = argv[2];
			Server server(port, password);

			struct sockaddr_in serverAddress;
			serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
			serverAddress.sin_family = AF_INET;
			serverAddress.sin_port = (server.getPort());

			bind(server.getSocketServer(), (const struct sockaddr *)&serverAddress, sizeof(serverAddress));
			cout << "bind: " << server.getSocketServer() << endl;

			listen(server.getSocketServer(), 5);
			cout << "listen" << endl;

			struct sockaddr_in addressUser;
			socklen_t csize = sizeof(addressUser);
			int SocketUser = accept(server.getSocketServer(), (struct sockaddr *)&addressUser, &csize);

			close(SocketUser);
			std::cout << server;
		}
	}
	catch ( exception &e )
	{
		cout << e.what() << endl;
	}
	return 0;
}