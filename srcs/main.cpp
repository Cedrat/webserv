#include "Server.hpp"
#include "Socket.hpp"
#include "Config.hpp"
#include "../config/ConfigParser.hpp"


int main()
{
   Server server;

   char file[] = "./config/step.conf";
    try
    {
         ConfigParser conf(file);
         for(int i = 0; i < conf.getServerNb(); i++)
            server.addConfig(conf.getOneServer(i));
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }


   server.createSocketsServer();
   server.launchingServer();
}