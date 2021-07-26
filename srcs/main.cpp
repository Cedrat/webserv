#include "Server.hpp"
#include "Socket.hpp"
#include "Config.hpp"

Location default_location()
{
   Location location;

   location.setAutoIndex(FALSE);
   location.addMethod("GET");
   location.setDefaultFile("index.html");
   location.setUploadFolder("/upload/");
   location.setRoot(".");
   location.setLocation(".");
   return (location);
}

Config default_config()
{
   Config config;
   Location location;

    config.setPrincipalServer(TRUE);
    config.setHost(0);
    config.setPort(7995);
    config.setServerOrClient(SERVER);
    config.setMaxBodySize(1024);
    config.addServerName("localhost");
    config.addErrorPages(400, "/error_files/err400.html");
    location = default_location();
    config.addErrorPages(404, "/error_files/err404.html");
    config.addErrorPages(505, "/error_files/err505.html");
    config.addLocation(location);
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