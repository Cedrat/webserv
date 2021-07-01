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
    if (strlen(filepath) <= 0)
        throw ("Error : The config file path is empty");

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
        {
            std::cerr << "Error in config file." << std::endl;
            return ;
        }

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
    std::vector<std::string>    line;
    bool                        closingBrace = false;

    while (_configFile.good())
    {
        line = FormattingLine(_configFile);

        /*for(unsigned long i = 0; i < line.size(); i++)
            std::cout << line[i] << std::endl;*/

        //Check derniere accolade
        if (line[0] == "}")
        {
            if (line.size() == 1)
                closingBrace = true;
            else
                std::cerr << "Error in config file : Syntax" << std::endl;
            return (-1);
        }
        
        //Check instruction serveur ou location
        if (line[0].isServerProperty())
        {
            //Envoyer vers la bonne fonction de serverConfig
                /*
                A faire
                */
        }
        else if (line[0] == "location")
        {
            //Envoyer vers la bonne fonction de locationConfig
                /*
                A faire
                */
        }
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
}




/***************************
Fonctions membres - Parsing
***************************/
void ConfigParser::parseServerName()
{
    //A déplacer dans server/locationConfig ?
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
    //Decouper : 1 elements = 1 vector -> Server,{,...
    if (!line.empty())
        formattedLine = splitLine(line);
    //Ou ajouter line.empty() -> continue; dans boucle pricnipale
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
        start = end + 1;
        end = line.find_first_of(" \n\t\r\f\v", end + 1);
    }
    formattedLine.push_back(line.substr(start, line.size()));

    return formattedLine;
}