#include "serverConfig.hpp"

serverConfig::serverConfig() 
{
    this->_host = -1;
    this->_port = -1;
    this->_max_body_size = -1;
    this->_server_names.push_back("-1");
    this->_root = "-1";
    this->_error_pages.insert(std::pair<int, std::string>(0, "0")); 
}

serverConfig::~serverConfig() { }




/***********************************************************
Parsing - listen
***********************************************************/
void serverConfig::setHostAndPort( std::vector<std::string> line )
{
    if (_port == -1 && _host == -1)
    {
        inet_pton(AF_INET, "0.0.0.1", &_host);
        this->_port = 80;
        if (line.size() == 1)
            return ;
        _tmpPortOrHost = line[1];
    }
    else
        throw ("Error : Config file can't contain more than one listen directive"); 
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
    if (_server_names[0] == "-1")
    {
        _server_names.clear();
        if (line.size() == 1)
        {
            _server_names[0] = "\"\"";
            return ;
        }
        for (size_t i = 1; i < line.size(); i++)
            this->_server_names.push_back(line[i]);
    }
    else
        throw ("Error : Config file can't contain more than one server_name directive");
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
//
//
//
//A ne pas oublier
//

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
    std::map<int, std::string>::iterator it = _error_pages.begin();

    if (it->first == 0 && it->second == "0")
        _error_pages.clear();

    error = atoi(line[1].c_str());
    path = line[2];

    _error_pages.insert(std::pair<int, std::string>(error, path));
}

bool serverConfig::checkErrorPages()
{
    std::map<int, std::string>::iterator it;

    for (it = _error_pages.begin(); it != _error_pages.end(); it++)
    {
        if (it->first < 300 || it->first > 599)
            return false;

        std::ifstream ifs;
        ifs.open(it->second.c_str(), std::ifstream::in);
        if (ifs.is_open() == false)
        {
            ifs.close();
            return false;
        }    
    }
    return true;
}






/***********************************************************
Parsing - max_client_body_size
***********************************************************/
void serverConfig::setMaxClientBodySize( std::vector<std::string> line )
{
    if (_max_body_size == -1)
    {
        if (line.size() == 1)
        {
            _max_body_size = 1000;
            return ;
        }
        this->_max_body_size = atoi(line[1].c_str());
    }
    else
        throw ("Error : Config file can't contain more than one client_max_body_size directive");
}

bool serverConfig::checkMaxClientBodySize()
{
    if (_max_body_size <= 0 || _max_body_size > 1200)
        return false;
    return true;
}




/***********************************************************
Parsing - root
***********************************************************/
void serverConfig::setRoot( std::vector<std::string> line )
{
    if (_root == "-1")
    {
        if (line.size() == 1)
        {
            _root = "./";
            return ;
        }
        this->_root = line[1];
    }
    else
        throw ("Error : Server block can't contain more than one root directive");
}

bool serverConfig::checkRoot()
{
    struct stat state;

    stat(_root.c_str(), &state);
    if (!S_ISDIR(state.st_mode))
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




void serverConfig::setLocation( std::vector<locationConfig> location )
{
    _locations = location;
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

std::string serverConfig::getRoot() const
{
    return this->_root;
}

std::map<int, std::string> serverConfig::getErrorPages() const
{
    return this->_error_pages;
}

std::vector<locationConfig> serverConfig::getLocations() const
{
    return this->_locations;
}

locationConfig serverConfig::getSpecificLocation( size_t id ) const
{
    if (_locations.size() < id)
        throw ("Error : Out of array location request");
    return this->_locations[id];
}



/*************************************************************
Config - checking
*************************************************************/
bool serverConfig::checkServerData()
{
    setUncalledDirectives();
    for (int i = 0; i < 5; i++)
    {
        if ((this->*_checks[i])() == false)
        {
            std::cerr << "Error in the server block configuration" << std::endl;
            return false;
        }
    }

    std::cout << "*** Debug server ***" << std::endl;
    std::cout << "Host : " << getHost() << "   Port : " << getPort() << std::endl;
    for (size_t i = 0; i < _server_names.size(); i++)
        std::cout << "server_name " << i << " - " <<  _server_names[i] << std::endl;
    std::cout << "Root : " << getRoot() << std::endl;
    std::cout << "max_body_size - " << _max_body_size << std::endl;
    std::map<int, std::string>::iterator it;
    for (it = _error_pages.begin(); it != _error_pages.end(); it++)
        std::cout << "error_page - " << it->first << " " << it->second << std::endl;
    std::cout << "*** End ***\n" << std::endl;

    return true;
}

void serverConfig::setUncalledDirectives()
{
    if (_port == -1)
        _port = 80;
    if (_host == -1)
        inet_pton(AF_INET, "0.0.0.1", &_host);
    if (_max_body_size == -1)
        _max_body_size = 1000;
    if (_root == "-1")
        _root = "./";
    if (_server_names[0] == "-1")
        _server_names[0] = "\"\"";
    if (_error_pages.size() == 1)
    {
        std::map<int, std::string>::iterator it = _error_pages.begin();
        if (it->first == 0 && it->second == "0")
        {
            _error_pages.clear();
            _error_pages.insert(std::pair<int, std::string>(404, "../www/errors/404.html"));
        }
    }
}