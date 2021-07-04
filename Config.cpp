#include "Config.hpp"

Config::Config()
{

}

Config::~Config()
{

}

int Config::getPort() const 
{
    return (_port);
}

void Config::setPort(int port)
{
    _port = port;
}

void Config::setServerOrClient(bool server_or_client)
{
    _server_or_client = server_or_client;
}

bool Config::getServerOrClient() const
{
    return (_server_or_client);
}