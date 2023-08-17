#include "server.hpp"

//int main( int argc, char **argv )
//{
//	try
//	{
//		if ( argc != 3 )
//			throw Server::BadArgument();
//		string password = argv[2];
//		Server server(atoi(argv[1]), password);
//		server.setSocketServer();
//		server.setAddressServer();
//		server.connectServer();
//
//		listen( server.getSocketServer(), 5 );
//		cout << "listen" << endl;
//
//		server.setAndAssignSocketToClient();
//	}
//	catch ( exception &e )
//	{
//		cout << e.what() << endl;
//	}
//
//
//	return (0);
//}
// ICI LE MAIN
int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage: " << argv[0] << " <port> <test>" << std::endl;
		return 1;
	}

	int port = atoi(argv[1]);
	std::string password = argv[2];

	try {
		Server server(port, password);
		server.setSocketServer();
		server.setAddressServer();
		server.connectServer();
		listen( server.getSocketServer(), 5 );
		server.setAndAssignSocketToClient();
	} catch (const char* errorMessage) {
		std::cerr << "Error: " << errorMessage << std::endl;
		return 1;
	}

	return 0;
}