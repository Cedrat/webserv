#include "serverConfig.hpp"

serverConfig::serverConfig() 
{
    inet_pton(AF_INET, "0.0.0.1", &_host);
    this->_port = 80;
    this->_max_body_size = 1000;
    this->_server_names.push_back("\"\"");
    this->_error_pages.insert(std::pair<int, std::string>(404, "./www/errors")); 
}

serverConfig::~serverConfig() { }




/***********************************************************
Parsing - listen
***********************************************************/
void serverConfig::setHostAndPort( std::vector<std::string> line )
{
    //Si il n'y a que le listen
    if (line.size() == 1)
        return ;
    
    _tmpPortOrHost = line[1]; 
}

bool serverConfig::checkHostAndPort()
{
    if (_tmpPortOrHost.empty())
        return true;

    //Chercher ':' && gestion si absent
    size_t separator = _tmpPortOrHost.find(":");
    if (separator == std::string::npos)
        return setOneHostOrPort(_tmpPortOrHost);
    
    //Transformer ce qu'il y a avant ':' en ip
    if (isIP(_tmpPortOrHost, ':') == true)
        inet_pton(AF_INET, _tmpPortOrHost.substr(0, separator).c_str(), &_host);
    else
        return false;

    //Assigner la valeur après ':' à port
    std::string  str = _tmpPortOrHost;
    str = _tmpPortOrHost.substr(separator + 1, (separator - _tmpPortOrHost.size()));  
    if (str == "" || isPort(static_cast<std::string>(str)) == false)
        return false;
    this->_port = atoi(str.c_str());
    return true;
}

bool serverConfig::setOneHostOrPort( std::string line )
{
    if (line == "localhost")
        inet_pton(AF_INET, "127.0.0.1", &_host);
    else if (isIP(line, '\0') == true)
        inet_pton(AF_INET, line.c_str(), &_host);
    else if (isPort(line) == true)
        this->_port = atoi(line.c_str());
    else
        return false;
    return true;
}


/**************************************************************
Parsing - server_name
**************************************************************/
void serverConfig::setServerNames( std::vector<std::string> line )
{
    if (line.size() == 1)
        return ;
    _server_names.clear();
    for (size_t i = 1; i < line.size(); i++)
        this->_server_names.push_back(line[i]);
}

bool serverConfig::checkServerNames()
{
    for (size_t i = 1; i < _server_names.size(); i++)
    {
        if (isAcceptableName(_server_names[i]) == false)
        {
            std::cerr << "Error in server_name declaration : "\
                << "Incorrect character in server name" << std::endl;
            return false;
        }
    }
    return true;
}

bool serverConfig::isAcceptableName( std::string line )
{
    int dbQuoteCounter = 0;

//TODO :
//Tous les checks

    for (int i = 0; line[i]; i++)
    {
        if (!isalnum(line[i]))
            return false;
        if (!isalpha(line[i]) && line[i] == '"')
            dbQuoteCounter++;
    }
    if (dbQuoteCounter % 2 != 0)
        return false;
    return true;
}




/********************
Parsing - error_page
********************/
void serverConfig::setErrorPages( std::vector<std::string> line )
{
    int error;
    std::string path;

    error = atoi(line[1].c_str());
    path = line[2];

    this->_error_pages.insert(std::pair<int, std::string>(error, path));
}

bool serverConfig::checkErrorPages()
{
    //Accéder à int pour check l'erreur
    //Puis check le path

    std::map<int, std::string>::iterator it;

    for (it = _error_pages.begin(); it != _error_pages.end(); it++)
    {
        //int = it->first
        //string = it->second
    }

}






/***********************************************************
Parsing - max_client_body_size
***********************************************************/
void serverConfig::setMaxClientBodySize( std::vector<std::string> line )
{
    if (line.size() == 1)
        return ;
    this->_max_body_size = atoi(line[1].c_str());
}

bool serverConfig::checkMaxClientBodySize()
{
    if (_max_body_size <= 0 || _max_body_size > 1200)
        return false;
    return true;
}



/*************************************************************
Utils
*************************************************************/
bool serverConfig::isIP( std::string line, char c )
{
    size_t start = 0;
    size_t dot_pos;
    size_t dot_nb = 0;

    if ((dot_pos = line.find("localhost")) != std::string::npos)
    {
        if (line.at(dot_pos + 9) != ':')
            return false;
        inet_pton(AF_INET, "127.0.0.1", &_host);
        return true;
    }
    for (int i = 0; line[i] != c; i++)
    {
        if (!isdigit(line[i]) && line[i] != '.')
            return false;
    }
    dot_pos = line.find(".");
    if (dot_pos == std::string::npos)
        return false;
    while (dot_pos != std::string::npos)
    {
        if (dot_pos - start <= 0 || dot_pos - start > 3)
            return false;
        dot_nb++;
        start = dot_pos + 1;
        dot_pos = line.find(".", start);
    }
    if ((line.find(":") != std::string::npos && line.find(":") - start > 3)
        || (line.find(":") == std::string::npos && line.size() - start > 3))
        return false;
    if (dot_nb == 3)
        return true;
    return false;
}

bool serverConfig::isPort( std::string line )
{
    for (int i = 0; line[i]; i++)
    {
        if (!isdigit(line[i]))
            return false;
    }
    return true;
}


/***********************************************************
Getters
***********************************************************/
int serverConfig::getPort() const
{
    return this->_port;
}

int serverConfig::getHost() const
{
    return this->_host;
}

std::vector<std::string> serverConfig::getServerNames() const
{
    return this->_server_names;
}

int serverConfig::getMaxClientBodySize() const
{
    return this->_max_body_size;
}







/*************************************************************
Config - checking
*************************************************************/
bool serverConfig::checkServerData()
{
    if (checkHostAndPort() == false)
    {
        std::cerr << "Error in listen directive" << std::endl;
        return false;
    }
    if (checkServerNames() == false)
    {
        std::cerr << "Error in server_name directive" << std::endl;
        return false;
    }
    if (checkErrorPages() == false)
    {
        std::cerr << "Error in error_page directive" << std::endl;
        return false;
    }
    if (checkMaxClientBodySize() == false)
    {
        std::cerr << "Error in max_client_body_size directive" << std::endl;
        return false;
    }


    std::cout << getHost() << " " << getPort() << std::endl;
    for (size_t i = 0; i < _server_names.size(); i++)
        std::cout << _server_names[i] << std::endl;
    std::cout << _max_body_size << std::endl;
    std::map<int, std::string>::iterator it;
    for (it = _error_pages.begin(); it != _error_pages.end(); it++)
        std::cout << it->first << " " << it->second << std::endl;

    return true;
}