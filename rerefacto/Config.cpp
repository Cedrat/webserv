#include "Config.hpp"

Config::Config() 
{
    this->_host = -1;
    this->_port = 0;
    this->_max_body_size = -1;
    this->_server_names.push_back("-1");
    this->_error_pages.insert(std::pair<int, std::string>(0, "0"));
    this->_principal_server = true;
    this->_server_or_client = SERVER;
}

Config::~Config() { }






/**************************************************************
Config - setting
**************************************************************/
void Config::setHostAndPort( std::vector<std::string> line )
{
    if (_port == 0 && _host == -1)
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
            {
                inet_pton(AF_INET, line[1].substr(0, separator).c_str(), &_host);
            }
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

void Config::setOneHostOrPort( std::string line )
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

void Config::setServerNames( std::vector<std::string> line )
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

void Config::setErrorPages( std::vector<std::string> line )
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

void Config::setMaxClientBodySize( std::vector<std::string> line )
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

void Config::setUncalledDirectives()
{
    if (_port == 0)
        _port = 80;
    if (_host == -1)
        inet_pton(AF_INET, "0.0.0.1", &_host);
    if (_max_body_size == -1)
        _max_body_size = 1000;
    if (_server_names[0] == "-1")
        _server_names[0] = "\"\"";
}

void Config::setPrincipalServer( bool value )
{
    this->_principal_server = value;
}

void Config::setServerOrClient(bool server_or_client)
{
    _server_or_client = server_or_client;
}




/*************************************************************
Config - checking
*************************************************************/
bool Config::checkServerData()
{
    setUncalledDirectives();

    checks _checks[3] = {&Config::checkServerNames, &Config::checkErrorPages, &Config::checkMaxClientBodySize};
    for (int i = 0; i < 3; i++)
    {
        if ((this->*_checks[i])() == false)
            return false;
    }
    debug();
    return true;
}

bool Config::checkServerNames()
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

bool Config::checkErrorPages()
{
    std::map<int, std::string>::iterator it = _error_pages.begin();

    if (it->first == 0 && it->second == "0")
        return true;

    for ( ; it != _error_pages.end(); it++)
    {
        if (it->first < 300 || it->first > 599)
            return false;

        if (!isAcceptableURI(it->second))
        {
            std::cerr << "Error in error_page directive : Invalid character" << std::endl;
            return false;
        }  
    }
    return true;
}

bool Config::checkMaxClientBodySize()
{
    if (_max_body_size <= 0 || _max_body_size > 1200)
    {
        std::cerr << "Error in max_client_body_size directive" << std::endl;
        return false;
    }
    return true;
}

bool Config::isEqual(const Config & rhs)
{
    if (_port == rhs.getPort() && _host == rhs.getHost())
    {
        std::vector<std::string> names = rhs.getServerNames();
        std::vector<std::string>::iterator it = _server_names.begin();
        for (std::vector<std::string>::iterator ite = _server_names.end(); it != ite; it++)
        {
            std::vector<std::string>::iterator rit = names.begin();
            for (std::vector<std::string>::iterator rite = names.end(); rit != rite; rit++)
            {
                if (*it == *rit)
                    return true;
            }
        }
    }
    return false;
}

//Duplicata de isEqual mais legetement different. A vor lequel garder ?
bool    Config::checkIfHostNameIsPresent(std::string host_name) const
{
   std::vector<std::string> host_names;
     
     host_names = getServerNames();

    std::vector<std::string>::iterator it_begin = host_names.begin();
    std::vector<std::string>::iterator it_end = host_names.end();


    for (int i = 0; it_begin != it_end; i++, it_begin++)
    {
        if (host_names[i] == host_name)
            return (TRUE);
    }
    if (host_name == ("127.0.0.1:" + int_to_string(getPort())) || host_name == ("localhost:" + int_to_string(getPort())))
        return (TRUE);
    return (FALSE); 
}



/***********************************************************
Getters
***********************************************************/
size_t Config::getPort() const
{
    return this->_port;
}

int Config::getHost() const
{
    return this->_host;
}

std::vector<std::string> Config::getServerNames() const
{
    return this->_server_names;
}

int Config::getMaxBodySize() const
{
    return this->_max_body_size;
}

std::map<int, std::string> Config::getErrorPages() const
{
    return this->_error_pages;
}

std::vector<Location> Config::getLocations() const
{
    return this->_locations;
}

Location Config::getOneLocation( size_t id ) const
{
    if (_locations.size() < id)
        throw std::out_of_range("Error : Out of array location request");
    return this->_locations[id];
}

void Config::setLocation( std::vector<Location> location )
{
    _locations = location;
}

bool Config::IsServerOrClient() const
{
    return (_server_or_client);
}

bool Config::IsPrincipalServer() const
{
    return (_principal_server);
}

std::string Config::getPathError(int num_error) const
{
    std::map<int, std::string>::const_iterator  it = _error_pages.find(num_error);
    if (it == _error_pages.end())
        return ("./www/default_error_files/default_err" + int_to_string(num_error) + ".html");
    return ("./www"+ it->second);
}












void Config::debug()
{
    std::cout << "*** Debug server ***" << std::endl;
    std::cout << "Is default server ? " << IsPrincipalServer() << std::endl;
    std::cout << "Is server or client ? " << IsServerOrClient() << std::endl;
    std::cout << "Host : " << getHost() << "   Port : " << getPort() << std::endl;
    if (_server_names.size() == 0)
        std::cout << "server_name 0 - " <<  _server_names[0] << std::endl;
    else
    {
        for (size_t i = 0; i < _server_names.size(); i++)
            std::cout << "server_name " << i << " - " <<  _server_names[i] << std::endl;
    }
    std::cout << "max_body_size - " << _max_body_size << std::endl;
    std::map<int, std::string>::iterator it;
    for (it = _error_pages.begin(); it != _error_pages.end(); it++)
        std::cout << "error_page - " << it->first << " " << it->second << std::endl;
    std::cout << "*** End ***\n" << std::endl;
}
