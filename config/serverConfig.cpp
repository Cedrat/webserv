#include "serverConfig.hpp"

serverConfig::serverConfig() 
{
    this->_host = -1;
    this->_port = -1;
    this->_max_body_size = -1;
    this->_server_names.push_back("-1");
    this->_root = "-1";
    this->_error_pages.insert(std::pair<int, std::string>(0, "0"));
    this->_default_server = false;
}

serverConfig::~serverConfig() { }






/**************************************************************
Config - setting
**************************************************************/
void serverConfig::setHostAndPort( std::vector<std::string> line )
{
    if (_port == -1 && _host == -1)
    {
        if (line.size() == 1)
            return ;

        //Chercher ':' && gestion si absent
        size_t separator = line[1].find(":");
        if (separator == std::string::npos)
            setOneHostOrPort(static_cast<std::string>(line[1]));
        else
        {
            //Assigner la valeur avant ':' à host
            if (isIP(line[1], ':', _host) == true)
                inet_pton(AF_INET, line[1].substr(0, separator).c_str(), &_host);
            else
                throw std::invalid_argument("Error : Config - Invalid host"); 
            
            //Assigner la valeur après ':' à port
            std::string  str = line[1].substr(separator + 1, (separator - line[1].size())); 
            if (str == "" || isPort(static_cast<std::string>(str)) == false)
                throw std::invalid_argument("Error : Config - Invalid port"); 
            this->_port = atoi(str.c_str());
        }
    }
    else
        throw std::invalid_argument("Error : Config file can't contain more than one listen directive"); 
}

void serverConfig::setOneHostOrPort( std::string line )
{
    if (line == "localhost")
        inet_pton(AF_INET, "127.0.0.1", &_host);
    else if (isIP(line, '\0', _host) == true)
        inet_pton(AF_INET, line.c_str(), &_host);
    else if (isPort(line) == true)
        this->_port = atoi(line.c_str());
    else
        throw std::invalid_argument("Error : Imvalid host or port");
}

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
        throw std::invalid_argument("Error : Config file can't contain more than one server_name directive");
}

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
        throw std::invalid_argument("Error : Config file can't contain more than one client_max_body_size directive");
}

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
        throw std::invalid_argument("Error : Server block can't contain more than one root directive");
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

void serverConfig::setDefaultServer( bool value )
{
    this->_default_server = value;
}




/*************************************************************
Config - checking
*************************************************************/
bool serverConfig::checkServerData()
{
    setUncalledDirectives();

    checks _checks[4] = {&serverConfig::checkRoot, &serverConfig::checkServerNames, 
                    &serverConfig::checkErrorPages, &serverConfig::checkMaxClientBodySize};
    for (int i = 0; i < 4; i++)
    {
        if ((this->*_checks[i])() == false)
            return false;
    }
    debug();
    return true;
}

bool serverConfig::checkServerNames()
{
    for (size_t i = 0; i < _server_names.size(); i++)
    {
        if (isAcceptableName(_server_names[i]) == false)
        {
            std::cerr << "Error in server_name directive" << std::endl;
            return false;
        }
    }
    return true;
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
            std::cerr << "Error in error_page directive" << std::endl;
            return false;
        }    
    }
    return true;
}

bool serverConfig::checkMaxClientBodySize()
{
    if (_max_body_size <= 0 || _max_body_size > 1200)
    {
        std::cerr << "Error in max_client_body_size directive" << std::endl;
        return false;
    }
    return true;
}

bool serverConfig::checkRoot()
{
    struct stat state;

    stat(_root.c_str(), &state);
    if (!S_ISDIR(state.st_mode))
    {
        std::cerr << "Error in root directive" << std::endl;
        return false;
    }
    return true;
}

bool serverConfig::isEqual(const serverConfig & rhs) const
{
    if (_port == rhs.getPort())
        return true;
    return false;
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

locationConfig serverConfig::getOneLocation( size_t id ) const
{
    if (_locations.size() < id)
        throw std::out_of_range("Error : Out of array location request");
    return this->_locations[id];
}

void serverConfig::setLocation( std::vector<locationConfig> location )
{
    _locations = location;
}

bool serverConfig::getDefaultServer() const
{
    return this->_default_server;
}





void serverConfig::debug()
{
    std::cout << "*** Debug server ***" << std::endl;
    std::cout << "Is default server ? " << getDefaultServer() << std::endl;
    std::cout << "Host : " << getHost() << "   Port : " << getPort() << std::endl;
    for (size_t i = 0; i < _server_names.size(); i++)
        std::cout << "server_name " << i << " - " <<  _server_names[i] << std::endl;
    std::cout << "Root : " << getRoot() << std::endl;
    std::cout << "max_body_size - " << _max_body_size << std::endl;
    std::map<int, std::string>::iterator it;
    for (it = _error_pages.begin(); it != _error_pages.end(); it++)
        std::cout << "error_page - " << it->first << " " << it->second << std::endl;
    std::cout << "*** End ***\n" << std::endl;
}

