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

ConfigParser::ConfigParser( char *filepath )
{
    _serverNb = 0;

    if (strlen(filepath) <= 0)
        throw ("Error : The config file path is empty");

    initServerProperties();
    initLocationProperties();

    std::string file = filepath;
    parser(file);
}

ConfigParser::~ConfigParser() { }


/***************************
Gestion centrale du parsing
***************************/
void ConfigParser::parser( std::string const & file )
{
    std::vector<std::string> line;
    //1. Ouvrir & check le fichier
    this->_configFile = openConfigFile(file);
    
    //2. Boucle principale 
    while (_configFile.good())
    {
        line = FormattingLine(_configFile);

        /*for(unsigned long i = 0; i < line.size(); i++)
            std::cout << line[i] << std::endl;*/

        if ((line.size() == 1 && line[0] != "server{")
            || (line.size() > 3) || (line.size() == 2 && line[1] != "{"))
        {
            std::cerr << "Error in config file." << std::endl;
            std::cerr << "Usage to start a server block is : 'Server {'" << std::endl;
            return ;
        }
        if (treatServerBlock() == -1)
            return ;
    }
}


/******************************
Fonctions membres - Check file
******************************/
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


/*************************************
Fonctions membres - Lecture des blocs
*************************************/
int ConfigParser::treatServerBlock()
{
    serverConfig                server;
    locationConfig              location;
    std::vector<std::string>    line;
    bool                        closingBrace = false;

    while (_configFile.good())
    {
        line = FormattingLine(_configFile);
        if (line.empty())
            continue ;
        /*for(unsigned long i = 0; i < line.size(); i++)
            std::cout << line[i] << std::endl;*/

        //Check derniere accolade
        if (line[0] == "}")
        {
            
            if (line.size() == 1)
            {
                //On ajoute le serveur complet a la liste
                closingBrace = true;
                _serverNb++;
                if (server.checkServerData() == false)
                    return (-1);
                _server.push_back(server);
                break ;
            }   
            else
            {
                std::cerr << "Error in config file : Syntax" << std::endl;
                return (-1);
            }    
        }
        
        //Check instruction serveur ou location
        if (isServerProperty(line[0]) == true)
        {
            //Envoyer vers la bonne fonction de serverConfig
            if (addServerProperty(line, &server) == false)
                return (-1);
        }
        //else if (line[0] == "location")
        //{
            //Envoyer vers la bonne fonction de locationConfig
                /*
                A faire
                */
        //}
        else
        {
            //1er argument pas reconnu ou pas suivi d'un espace
            std::cerr << "Error in config file : Unrecognized or out of line keyword " \
                << line[0] << std::endl;
            std::cerr << "NB : Keywords must be followed by a whitespace" << std::endl;
            return (-1);
        }
    }

    if (closingBrace == false)
    {
        std::cerr << "Error in config file : No closing bracket" << std::endl;
        return (-1);
    }
    //Finaliser la config ici ?

    return (1);
}


/*******************************
Fonctions membres - Utilitaires
*******************************/
std::vector<std::string> ConfigParser::FormattingLine( std::ifstream & file )
{
    std::string                 line;
    std::vector<std::string>    formattedLine; 

    std::getline(file, line);
    //Enlever commentaires & espaces avant/après
    line = trimComment(line);
    line = trimStartAndEndWhitespaces(line);
    line = trimDotComa(line);
    //Decouper : 1 elements = 1 vector -> Server,{,...
    if (!line.empty())
        formattedLine = splitLine(line);
    //Ou ajouter line.empty() -> continue; dans boucle principale
    return formattedLine;
}

std::string ConfigParser::trimComment( std::string line )
{
    std::string newLine;
    size_t      commentPos;

    commentPos = line.find("#");
    if (commentPos == std::string::npos)
        return line;
    else
        newLine = line.substr(0, commentPos);

    return newLine;
}

std::string ConfigParser::trimStartAndEndWhitespaces( std::string line )
{
    std::string newLine;
    size_t      whitePos;

    whitePos = line.find_first_not_of(" \n\t\r\f\v");
    if (whitePos != std::string::npos)
        newLine = line.substr(whitePos);

    whitePos = newLine.find_last_not_of(" \n\t\r\f\v");
    if (whitePos != std::string::npos)
        newLine = newLine.substr(0, whitePos + 1);

    return newLine;
}

std::string ConfigParser::trimDotComa( std::string line )
{
    std::string newLine;
    size_t      signPos;
    size_t      accoladePos;

    if ((accoladePos = line.find("{")) != std::string::npos || (accoladePos = line.find("}")) != std::string::npos)
        return line;

    signPos = line.find(";");
    if (!line.empty() && signPos == std::string::npos)
        throw ("Error : Line need to be terminated with a single ';'");
    else if (line[signPos + 1])
        throw ("Error : Line need to be terminated with a single ';'");
    else
        newLine = trimStartAndEndWhitespaces(line.substr(0, signPos));
    
    return newLine;
}

std::vector<std::string> ConfigParser::splitLine( std::string line )
{
    //Renvoie vector<string> avec les éléments découpés 
    //selon les espaces
    std::vector<std::string>    formattedLine; 
    size_t                      start = 0, end = 0;

    end = line.find_first_of(" \n\t\r\f\v");
    if (end == std::string::npos)
    {
        formattedLine.push_back(line);
        return formattedLine;
    }
    while (end != std::string::npos)
    {
        formattedLine.push_back(line.substr(start, (end - start)));
        if (line[end + 1] == ' ')
        {
            start = line.find_first_not_of(" \n\t\r\f\v", end + 1);
            end = line.find_first_of(" \n\t\r\f\v", start + 1);
        }
        else
        {        
            start = end + 1;
            end = line.find_first_of(" \n\t\r\f\v", end + 1);
        }
    }
    formattedLine.push_back(line.substr(start, line.size()));

    return formattedLine;
}



/****************************************
Fonctions membres - Gestion des keywords
****************************************/
void ConfigParser::initServerProperties()
{
    this->_serverProperties.push_back("listen");
    this->_serverProperties.push_back("server_name");
    this->_serverProperties.push_back("error_page");
    this->_serverProperties.push_back("client_max_body_size");
}

void ConfigParser::initLocationProperties()
{
    this->_locationProperties = {"root", "methods", "autoindex", "index", "upload_folder", "cgi"};
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
    {
        server->setHostAndPort(line);
    }  
    else if (line[0] == "server_name")
        server->setServerNames(line);
    else if (line[0] == "error_page" && line.size() == 3)
        server->setErrorPages(line);
    else if (line[0] == "client_max_body_size" && line.size() == 2)
        server->setMaxClientBodySize(line);
    else
    {
        std::cerr << "Error in config file : Wrong or missing argument in server block" << std::endl;
        return false;
    }
    return true;
}