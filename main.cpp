#include "Server.hpp"
#include "Socket.hpp"
#include "Config.hpp"

int main()
{
    Server server;
    Socket socket;
    Config config;
    
    config.setPort(7995);
    config.setServerOrClient(SERVER);  
    server.addSocketServer(config);
    server.launchingServ();
}