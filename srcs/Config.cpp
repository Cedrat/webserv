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

void Config::setPrincipalServer(bool true_or_false)
{
    _principal_server = true_or_false;
}

void Config::setHost(int host)
{
    _host = host;
}

void Config::setMaxBodySize(int max_body_size)
{
    _max_body_size = max_body_size;
}

void Config::addServerName(std::string str)
{
    _server_names.push_back(str);
}

void Config::addErrorPages(int error_nb, std::string path_error)
{
    _error_pages.insert(std::pair<int, std::string> (error_nb, path_error));
}

void Config::addLocation(Location location)
{
    _locations.push_back(location);    
}

int Config::getHost(void) const
{
    return (_host);
}

int Config::getMaxBodySize(void) const
{
    return (_max_body_size);
}

std::vector<std::string> Config::getServersNames(void) const
{
    return (_server_names);
}

std::map<int, std::string> Config::getErrorPages(void) const
{
    return (_error_pages);
}

std::vector<Location> Config::getLocations() const
{
    return (_locations);
}

bool    Config::IsServerOrClient() const
{
    return (_server_or_client);
}

bool    Config::IsPrincipalServer() const
{
    return (_principal_server);
}

