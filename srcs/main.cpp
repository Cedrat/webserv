
#include "ConfigParser.hpp"
#include "Server.hpp"
#include "EOFException.hpp"

int main()
{
	Server server;
//test
   char file[] = "./srcs/step.conf";
	try
	{
		ConfigParser conf(file);
		for(int i = 0; i < conf.getServerNb(); i++)
			server.addConfig(conf.getOneServer(i));
	}
	catch(char const* & e)
	{
		std::cerr << e << std::endl;
	}

	try {
		server.createSocketsServer();
	}
	catch(EmergencyExit const& e)
	{
		server.endServer();
		//Clean server
		return 0;
	}
	try{
		server.launchingServer();
	}
	catch (EmergencyExit const &e)
	{
		std::cout << "Server has quitted ! Good Night ! " << std::endl;
		return (0);
	}
}
