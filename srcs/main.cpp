<<<<<<< HEAD
#include "Server.hpp"
#include "Socket.hpp"
#include "Config.hpp"
#include "../config/ConfigParser.hpp"

=======

#include "ConfigParser.hpp"
#include "Server.hpp"
>>>>>>> refacto

int main()
{
   Server server;

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


<<<<<<< HEAD
   server.createSocketsServer();
=======
   //Config config;
   //config = default_config();
   //server.addConfig(config);
   try {
        server.createSocketsServer();
   }
    catch(char const * &e)
    {
        std::cerr << e << std::endl;
    }
>>>>>>> refacto
   server.launchingServer();
}