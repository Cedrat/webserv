#include "MethodCgi.hpp"

MethodCgi::MethodCgi(int fd, std::string path, std::string header, Config config, Location location, std::string body) 
    : AMethod(fd, path, header), _config(config), _location(location), _body(body)
{

}

MethodCgi::~MethodCgi()
{

}

void MethodCgi::init()
{
    //Initialisation des env et creation du fichier tmp ?
}

void MethodCgi::exec()
{
    //La magie se passe ici (execution du cgi)
}