#include "headers/server.hpp"


int main(int argc, char** argv){

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
			server.createSocketServer();

			server.waitToNewConnection();
			//close(SocketUser);
			std::cout << server;
		}
	}
	catch ( exception &e )
	{
		cout << e.what() << endl;
	}
	return 0;

}