#include "Server.hpp"
#include "Socket.hpp"
#include "Config.hpp"

Location default_location()
{
   Location location;

   location.setAutoIndex(FALSE);
   return (location);
}

Config default_config()
{
   Config config;
   Location location;

    config.setHost(0);
    config.setPort(7995);
    config.setServerOrClient(SERVER);
    config.setMaxBodySize(1024);
    return (config);
}

int main()
{
    Server server;
    Socket socket;
    Config config;
    Config config2;
    
    config2.setPort(7996);
    config2.setServerOrClient(SERVER);
    server.addSocketServer(config2);
    config = default_config();
    server.addSocketServer(config);
    server.launchingServ();
}