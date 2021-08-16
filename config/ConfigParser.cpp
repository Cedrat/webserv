/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchampda <dchampda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/28 15:09:32 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/28 21:26:27 by dchampda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

ConfigParser::ConfigParser( char *filepath ) : _serverNb(0)
{
    if (strlen(filepath) <= 0)
        throw std::invalid_argument("Error : The config file path is empty");

    std::string file = filepath;
    parser(file);
}

ConfigParser::~ConfigParser() { }






/************************************************************
Gestion centrale du parsing
************************************************************/
void ConfigParser::parser( std::string const & file )
{
    std::vector<std::string> line;
    this->_configFile = openConfigFile(file);
    
    while (_configFile.good())
    {
        line = FormattingLine(_configFile);
        if (line.empty())
            continue;
        if ((line.size() == 1 && line[0] != "server{")
            || (line.size() > 3) || (line.size() == 2 && line[1] != "{"))
        {
            std::cerr << "Error in config file." << std::endl;
            std::cerr << "Usage to start a server block is : 'Server {'" << std::endl;
            return ;
        }
        if (treatServerBlock() == false)
            return ;
    }
}

std::ifstream ConfigParser::openConfigFile( std::string const & file )
{
    std::ifstream ifs;
    size_t extPos = file.find(".conf", 0);

    if (extPos == std::string::npos || (extPos != file.size() - 5)
        || file.size() <= 5)
        throw std::invalid_argument("Error : Invalid configuration file format");

    ifs.open(file, std::ifstream::in);
    if (ifs.is_open() == false)
        throw std::invalid_argument("Error : Can't open file");
    
    return (ifs);
}


/**************************************************************
 Lecture des blocs
**************************************************************/
bool ConfigParser::treatServerBlock()
{
    serverConfig                server;
    std::vector<std::string>    line;
    bool                        closingBrace = false;

    while (_configFile.good())
    {
        line = FormattingLine(_configFile);
        if (line.empty())
            continue ;

        if (line[0] == "}") //Check derniere accolade
        {
            closingBrace = closeServerBlock(line, &server);
            break;
        } 
        else if (isServerProperty(line[0]) == true) //Envoyer vers la bonne fonction de serverConfig
        {
            if (addServerProperty(line, &server) == false)
                return false;
        }
        else if (isLocation(line) == true) //Envoyer vers la bonne fonction de locationConfig
        {
            if (treatLocationBlock(line) == false)
                return false;
        }
        else //1er argument pas reconnu ou pas suivi d'un espace
            throw std::invalid_argument("Error in the config file : Unrecognized or out of line keyword '" + line[0] + "'");
    }

    if (closingBrace == false)
        throw std::invalid_argument("Can't create the server : Error in the server block of the config file");
    return true;
}


bool ConfigParser::treatLocationBlock( std::vector<std::string> line )
{
    locationConfig location;
    bool closingBrace = false;

    location.setLocationDirective(line);    
    while (_configFile.good())
    {
        line = FormattingLine(_configFile);
        if (line.empty())
            continue ;
        
        //Check derniere accolade
        if (line[0] == "}")
        {
            closingBrace = closeLocationBlock(line, &location);
            break ;
        }
        else if (isLocationProperty(line[0]) == true)
        {
            if (addLocationProperty(line, &location) == false)
                return false;
        }
        else //1er argument pas reconnu ou pas suivi d'un espace
            throw std::invalid_argument("Error in the config file : Unrecognized or out of line keyword '" + line[0] + "'");
    }

    if (closingBrace == false)
    {
        throw std::invalid_argument("Can't create the server : Error in the location block of the config file");
    }
    return true;
}



/**************************************************************
Enregistrement des structures de données
**************************************************************/
bool ConfigParser::closeServerBlock( std::vector<std::string> line, serverConfig * server )
{
    //if (_serverNb < 100) ?

    if (line.size() == 1)
    {
        //On ajoute le serveur complet a la liste & les blocs location
        if (server->checkServerData() == false)
            return false;
        for (int i = 0; i < _serverNb; i++)
        {
            if (server->isEqual(_server[i]))
                throw std::invalid_argument("Server bloc duplicata");
        }
        _server.push_back(*server);
        _server[_serverNb].setLocation(_location);
        _location.clear();
        //std::cout << _server[0].getOneLocation(0).getLocation() << std::endl;
        _serverNb++;
        return true;
    }   
    else
    {
        std::cerr << "Error in config file : Syntax" << std::endl;
        return false;
    }    
}

bool ConfigParser::closeLocationBlock( std::vector<std::string> line, locationConfig * location )
{
    if (line.size() == 1)
    {
        if (location->checkLocationData() == false)
            return false;
        for (int i = 0; i < _locationNb; i++)
        {
            if (location->isEqual(_location[i]))
                throw std::invalid_argument("Location bloc duplicata");
        }
        _location.push_back(*location);
        _locationNb++;
        return true;
    }   
    else
    {
        std::cerr << "Error in config file : Syntax" << std::endl;
        return false;
    }
 }



/**************************************************************
Utilitaires
**************************************************************/
std::vector<std::string> ConfigParser::FormattingLine( std::ifstream & file )
{
    std::string                 line;
    std::vector<std::string>    formattedLine; 

    std::getline(file, line);
    //Enlever commentaires & espaces avant/après & point virgule
    line = trimComment(line);
    line = trimStartAndEndWhitespaces(line);
    line = trimDotComa(line);
    //Decouper : 1 elements = 1 vector -> Server,{,listen,...
    if (!line.empty())
        formattedLine = splitLine(line);
    return formattedLine;
}

bool ConfigParser::isLocation( std::vector<std::string> line )
{
    if (line[0] != "location")
        return false;
    if ((line.size() != 3) || (line.size() == 3 && line[2] != "{"))
    {
        std::cerr << "Error in location directive : Missing path or opening bracket" << std::endl;
        return false;
    }
    return true;
}


/**************************************************************
Gestion des keywords
**************************************************************/
bool ConfigParser::isServerProperty( std::string line )
{
    std::vector<std::string> serverProperties = {"listen", "server_name", "error_page", "client_max_body_size", "root"};
    
    for(unsigned long i = 0; i < serverProperties.size(); i++)
    {
        if (line == serverProperties[i])
            return true;
    }
    return false;
}

bool ConfigParser::isLocationProperty( std::string line )
{
    std::vector<std::string> locationProperties = {"root", "method", "autoindex", "index", "upload_folder", "cgi"};
   
    for(unsigned long i = 0; i < locationProperties.size(); i++)
    {
        if (line == locationProperties[i])
            return true;
    }
    return false;
}

bool ConfigParser::addServerProperty( std::vector<std::string> line, serverConfig * server )
{
    if (line[0] == "listen" && line.size() <=  2)
        server->setHostAndPort(line);
    else if (line[0] == "server_name")
        server->setServerNames(line);
    else if (line[0] == "error_page" && line.size() == 3)
        server->setErrorPages(line);
    else if (line[0] == "client_max_body_size" && line.size() == 2)
        server->setMaxClientBodySize(line);
    else if (line[0] == "root" && line.size() <=  2)
        server->setRoot(line);
    else
    {
        std::cerr << "Error in config file : Wrong or missing argument in server block" << std::endl;
        return false;
    }
    return true;
}

bool ConfigParser::addLocationProperty( std::vector<std::string> line, locationConfig * location )
{
    if (line[0] == "root" && line.size() < 3)
        location->setRoot(line);
    else if (line[0] == "autoindex" && line.size() < 3)
        location->setAutoindex(line);
    else if (line[0] == "method")
        location->setMethods(line);
    else if (line[0] == "index" && line.size() == 2)
        location->setIndex(line);
    else if (line[0] == "upload_folder" && line.size() < 3)
        location->setUploadFolder(line);
    else if (line[0] == "cgi" && line.size() == 3)
        location->setCgi(line);
    else
    {
        std::cerr << "Error in config file : Wrong or missing argument in location block" << std::endl;
        return false;
    }
    return true;
}