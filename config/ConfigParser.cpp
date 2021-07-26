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
        throw ("Error : The config file path is empty");

    initServerProperties();
    initLocationProperties();

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
        throw ("Error : Invalid configuration file format");

    ifs.open(file, std::ifstream::in);
    if (ifs.is_open() == false)
        throw ("Error : Can't open file");
    
    return (ifs);
}


/**************************************************************
Fonctions membres - Lecture des blocs
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

        //Check derniere accolade
        if (line[0] == "}")
        {
            closingBrace = closeServerBlock(line, &server);
            break;
        } 
        else if (isServerProperty(line[0]) == true) //Envoyer vers la bonne fonction de serverConfig
        {
            if (addServerProperty(line, &server) == false)
                return false;
        }
        else if (line[0] == "location") //Envoyer vers la bonne fonction de locationConfig
        {
            if (treatLocationBlock(line) == false)
                return false;
        }
        else //1er argument pas reconnu ou pas suivi d'un espace
        {
            std::cerr << "Error in config file : Unrecognized or out of line keyword " \
                << line[0] << std::endl;
            return false;
        }
    }

    if (closingBrace == false)
    {
        std::cerr << "Error in config file : No closing bracket" << std::endl;
        return false;
    }
    return true;
}



bool ConfigParser::treatLocationBlock( std::vector<std::string> line )
{
    locationConfig * location = new locationConfig;
    bool closingBrace = false;

    if (line.size() > 3 || line.size() < 3 || (line.size() == 3 && line[2] != "{"))
    {
        std::cerr << "Error in location directive : Missing path or opening bracket" << std::endl;
        return false;
    }
    location->setLocationDirective(line);
    
    while (_configFile.good())
    {
        line = FormattingLine(_configFile);
        if (line.empty())
            continue ;
        
        //Check derniere accolade
        if (line[0] == "}")
        {
            closingBrace = closeLocationBlock(line, location);
            break ;
        }
        else if (isLocationProperty(line[0]) == true)
        {
            if (addLocationProperty(line, location) == false)
                return false;
        }
        else //1er argument pas reconnu ou pas suivi d'un espace
        {
            std::cerr << "Error in config file : Unrecognized or out of line keyword " \
                << line[0] << std::endl;
            return false;
        }
    }

    if (closingBrace == false)
    {
        std::cerr << "Error in config file : No closing bracket in location block" << std::endl;
        return false;
    }
    return true;
}

bool ConfigParser::closeServerBlock( std::vector<std::string> line, serverConfig * server )
{
    if (line.size() == 1)
    {
        //On ajoute le serveur complet a la liste & les blocs location
        if (server->checkServerData() == false)
            return false;
        _server.push_back(*server);
        _server[_serverNb].setLocation(_location);
        //std::cout << _server[0].getSpecificLocation(0).getLocation() << std::endl;
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
        _location.push_back(*location);
        _locationNb++;
        delete location;
        return true;
    }   
    else
    {
        std::cerr << "Error in config file : Syntax" << std::endl;
        return false;
    }
 }


/*******************************
Fonctions membres - Utilitaires
*******************************/
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


/****************************************
Fonctions membres - Gestion des keywords
****************************************/
void ConfigParser::initServerProperties()
{
    this->_serverProperties = {"listen", "server_name", "error_page", "client_max_body_size", "root"};
}

void ConfigParser::initLocationProperties()
{
    this->_locationProperties = {"root", "method", "autoindex", "index", "upload_folder", "cgi"};
}

bool ConfigParser::isServerProperty( std::string line )
{
    for(unsigned long i = 0; i < _serverProperties.size(); i++)
    {
        if (line == _serverProperties[i])
            return true;
    }
    return false;
}

bool ConfigParser::isLocationProperty( std::string line )
{
    for(unsigned long i = 0; i < _locationProperties.size(); i++)
    {
        if (line == _locationProperties[i])
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