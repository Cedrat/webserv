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

#include "../config/ConfigParser.hpp"

ConfigParser::ConfigParser(const char *filepath ) : _serverNb(0), _locationNb(0)
{
	if (strlen(filepath) <= 0)
		throw std::invalid_argument("Error : The config file path is empty");

	std::string file = filepath;
	try {
		parser(file);
	}
    catch(const std::invalid_argument& ia){
        std::cerr << ia.what() << std::endl;
        throw(ParserExit());
    }
}

ConfigParser::~ConfigParser() { }

ConfigParser::ConfigParser( ConfigParser const & src )
{
	this->_serverNb = src._serverNb;
	this->_locationNb = src._locationNb;
	this->_server = src._server;
	this->_location = src._location;
}

ConfigParser & ConfigParser::operator=( ConfigParser const & rhs )
{
	if (this != &rhs)
	{
		this->_serverNb = rhs._serverNb;
		this->_locationNb = rhs._locationNb;
		if (this->_server.empty() == FALSE)
			this->_server.clear();
		this->_server = rhs._server;
		if (this->_location.empty() == FALSE)
			this->_location.clear();
		this->_location = rhs._location;
	}
	return *this;
}




/************************************************************
Gestion centrale du parsing
************************************************************/
void ConfigParser::parser( std::string const & file )
{
	std::vector<std::string> line;
	openConfigFile(file);

	while (_configFile.good())
	{
		line = FormattingLine(_configFile);
		if (line.empty())
			continue;
		if ((line.size() == 1 && line[0] != "server{")
			|| (line.size() > 3) || (line.size() == 2 && (line[0] != "server" || line[1] != "{")))
		{
			throw std::invalid_argument("Error in config file.\nUsage to start a server block is : 'Server {'");
			return ;
		}
		if (treatServerBlock() == false)
			throw std::invalid_argument("Error in config file.");
	}
	if (_serverNb <= 0)
		throw std::invalid_argument("Error : Config file is empty");
}

void ConfigParser::openConfigFile( std::string const & file )
{
	size_t extPos = file.find(".conf", 0);

	if (extPos == std::string::npos || (extPos != file.size() - 5)
		|| file.size() <= 5)
		throw std::invalid_argument("Error : Invalid configuration file format");

	this->_configFile.open(file.c_str(), std::ifstream::in);
	if (this->_configFile.is_open() == false)
	{
		throw std::invalid_argument("Error : Can't open file");
	}
}


/**************************************************************
 Lecture des blocs
**************************************************************/
bool ConfigParser::treatServerBlock()
{
	Config                      server;
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
		else if (isLocation(line) == true) //Envoyer vers la bonne fonction de Location
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
	Location location;
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
		throw std::invalid_argument("Can't create the server : Error in the location block of the config file");
	return true;
}



/**************************************************************
Enregistrement des structures de donn??es
**************************************************************/
bool ConfigParser::closeServerBlock( std::vector<std::string> line, Config * server )
{
	if (line.size() == 1)
	{
		//On defini serveur par defaut, on check les donnees et absence de doublon
		//On ajoute le serveur complet a la liste & les blocs location
		if (server->checkServerData() == false)
			return false;
		for (int i = 0; i < _serverNb; i++)
		{
			if (server->isEqual(_server[i]))
				throw std::invalid_argument("Server bloc duplicata");
			if ((_server[i].getPort() == server->getPort()) && server->IsPrincipalServer() == true && _server[i].IsPrincipalServer() == true)
				server->setPrincipalServer(false);
		}
		_server.push_back(*server);
		_server[_serverNb].setLocation(_location);
		if (_locationNb == 0)
			throw std::invalid_argument("You need at least one location bloc");
		_location.clear();
		_locationNb = 0;
		_serverNb++;
		return true;
	}   
	else
	{
		std::cerr << "Error in config file : Syntax" << std::endl;
		return false;
	}    
}

bool ConfigParser::closeLocationBlock( std::vector<std::string> line, Location * location )
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
	//Enlever commentaires & espaces avant/apr??s & point virgule
	try {
		line = trimComment(line);
		line = trimStartAndEndWhitespaces(line);
		line = trimDotComa(line);
	}
	catch(char const* & e) {
		std::cerr << e << std::endl;
		throw(ParserExit());
	}
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
	std::string serverProperties[SERVER_DIRECTIVE_NB] = {"listen", "server_name", "error_page", "client_max_body_size"};
	
	for(unsigned long i = 0; i < SERVER_DIRECTIVE_NB; i++)
	{
		if (line == serverProperties[i])
			return true;
	}
	return false;
}

bool ConfigParser::isLocationProperty( std::string line )
{
	std::string locationProperties[LOCATION_DIRECTIVE_NB] = {"root", "method", "autoindex", "index", "upload_folder", "cgi", "rewrite"};
   
	for(unsigned long i = 0; i < LOCATION_DIRECTIVE_NB; i++)
	{
		if (line == locationProperties[i])
			return true;
	}
	return false;
}

bool ConfigParser::addServerProperty( std::vector<std::string> line, Config * server )
{
	if (line[0] == "listen" && line.size() <=  2)
		server->setHostAndPort(line);
	else if (line[0] == "server_name")
		server->setServerNames(line);
	else if (line[0] == "error_page" && line.size() == 3)
		server->setErrorPages(line);
	else if (line[0] == "client_max_body_size" && line.size() == 2)
		server->setMaxClientBodySize(line);
	else
	{
		std::cerr << "Wrong or missing argument in server block" << std::endl;
		return false;
	}
	return true;
}

bool ConfigParser::addLocationProperty( std::vector<std::string> line, Location * location )
{
	if (line[0] == "root" && line.size() == 2)
		location->setRoot(line);
	else if (line[0] == "autoindex" && line.size() == 2)
		location->setAutoindex(line);
	else if (line[0] == "method" && line.size() >= 2)
		location->setMethods(line);
	else if (line[0] == "index" && line.size() == 2)
		location->setDefaultFile(line);
	else if (line[0] == "upload_folder" && line.size() == 2)
		location->setUploadFolder(line);
	else if (line[0] == "cgi" && line.size() == 3)
		location->setCgi(line);
	else if (line[0] == "rewrite" && line.size() == 2)
		location->setRedirect(line);
	else
	{
		std::cerr << "Error in config file : Wrong or missing argument in location block" << std::endl;
		return false;
	}
	return true;
}



/**************************************************************
Getters
**************************************************************/
std::vector<Config> ConfigParser::getServer()
{
	return this->_server;
}

Config ConfigParser::getOneServer( size_t i )
{
	return _server[i];
}

int ConfigParser::getServerNb()
{
	return this->_serverNb;
}