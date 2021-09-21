
#include "ConfigParser.hpp"
#include "Server.hpp"

int main()
{
   Server server;

   char file[] = "./step.conf";
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
   server.launchingServer();
}