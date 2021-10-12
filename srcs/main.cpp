
#include "ConfigParser.hpp"
#include "Server.hpp"
#include "EOFException.hpp"

std::string config_path(int argc, char **argv)
{
	if (argc == 2)
	{
		std::string path(argv[1]);
		return (path);
	}
	else if (argc == 1)
	{
		std::string path("./srcs/step.conf");
		return (path);
	}
	else
	{
		return ("");
	}
}

int main(int argc, char **argv)
{
	Server server;
	
	std::string file(config_path(argc, argv));
	if (file == "")
	{
		std::cerr << "./webserv need zero or one argument:\n0: default.conf will be used.\n1: give me a good config file!" << std::endl;
		return EXIT_FAILURE;
	}
	try
	{
		ConfigParser conf(file.c_str());
		for(int i = 0; i < conf.getServerNb(); i++)
			server.addConfig(conf.getOneServer(i));
	}
	catch(std::exception const & e)
	{
		std::cout << e.what() << std::endl;
		return (0);
	}

	try {
		server.createSocketsServer();
	}
	catch(EmergencyExit const& e)
	{
		server.endServer();
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
