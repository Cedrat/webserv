
#include "ConfigParser.hpp"
#include "Server.hpp"
#include "CustomException.hpp"

int main()
{
	Server server;

   char file[] = "./srcs/step.conf";
	try
	{
		ConfigParser conf(file);
		for(int i = 0; i < conf.getServerNb(); i++)
			server.addConfig(conf.getOneServer(i));
		std::cout << "Everything went well :)" << std::endl;
	}
	catch(ParserExit const& e)
	{
		return 0;
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
