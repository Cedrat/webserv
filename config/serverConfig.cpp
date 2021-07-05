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


/****************
Parsing - listen
****************/
bool serverConfig::setHostAndPort( std::vector<std::string> line )
{
    size_t       separator;

    //Si il n'y a que le listen
    if (line.size() == 1)
        return true;
    //Chercher ':' && gestion si absent
    separator = line[1].find(":");
    if (separator == std::string::npos)
        return setOneHostOrPort(line[1]);

    //Transformer ce qu'il y a avant ':' en ip
    if (isIP(line[1], ':') == true)
        inet_pton(AF_INET, line[1].substr(0, separator).c_str(), &_host);
    else
        return false;

    //Assigner la valeur après ':' à port
    std::string  str = line[1];
    str = line[1].substr(separator + 1, (separator - line[1].size()));  
    if (str == "" || isPort(static_cast<std::string>(str)) == false)
        return false;
    this->_port = atoi(str.c_str());
    return true;
}

/************************
Utils - listen directive
************************/
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



/*********************
Parsing - server_name
*********************/
bool serverConfig::setServerNames( std::vector<std::string> line )
{
    //Si il n'y a pas de nom de serveur
    if (line.size() == 1)
        return true;
    
    for (size_t i = 1; i < line.size(); i++)
    {
        if (isAcceptableChar(line[i]) == false)
        {
            std::cerr << "Error in server_name declaration : "\
                << "Incorrect character in server name" << std::endl;
            return false;
        }
            
        this->_server_names.push_back(line[i]);
        std::cout << this->_server_names[i] << std::endl;
    }
    return true;
}

bool serverConfig::isAcceptableChar( std::string line )
{
    int dbQuoteCounter = 0;

//Voir pour accepter les chiffres en dehors des ip ?
    for (int i = 0; line[i]; i++)
    {
        if (isalpha(line[i]))
            continue ;
        if (!isalpha(line[i]) && line[i] == '"')
            dbQuoteCounter++;
        else if ((isdigit(line[i]) && isIP(line, '\0')) || (!isalpha(line[i]) && line[i] == '.')
            || (ispunct(line[i]) && isIP(line, '\0')))
            return true;
        else
            return false;
    }
    if (dbQuoteCounter % 2 != 0)
        return false;
    return true;
}


/********************
Parsing - error_page
********************/


/******************************
Parsing - max_client_body_size
******************************/




/*****
Utils
*****/
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


/*******
Getters
*******/
int serverConfig::getPort() const
{
    return this->_port;
}

int serverConfig::getHost() const
{
    return this->_host;
}