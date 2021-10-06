#include "Location.hpp"

Location::Location() : _root(UNSET), _autoindex(FALSE), _set_autoindex(FALSE),
						_default_file(UNSET), _upload_folder(UNSET), _redirect(UNSET)
{
	this->_methods.push_back(UNSET);
	this->_cgi.insert(std::pair<std::string, std::string>("0", "0"));
}

Location::~Location() { }

Location::Location( Location const & src )
{
	this->_location = src.getLocation();
	this->_root = src.getRoot();
	this->_methods = src.getMethods();
	this->_autoindex = src.getAutoIndex();
	this->_set_autoindex = src._set_autoindex;
	this->_default_file = src.getDefaultFile();
	this->_upload_folder = src.getUploadFolder();
	this->_redirect = src.getRedirect();
	this->_cgi = src.getCgi();
}

Location & Location::operator=( Location const & rhs )
{
	if (this != &rhs)
	{
		this->_location = rhs.getLocation();
		this->_root = rhs.getRoot();
		if (this->_methods.empty() == FALSE)
			this->_methods.clear();
		this->_methods = rhs.getMethods();
		this->_autoindex = rhs.getAutoIndex();
		this->_set_autoindex = rhs._set_autoindex;
		this->_default_file = rhs.getDefaultFile();
		this->_upload_folder = rhs.getUploadFolder();
		this->_redirect = rhs.getRedirect();
		if (this->_cgi.empty() == FALSE)
			this->_cgi.clear();
		this->_cgi = rhs.getCgi();
	}
	return *this;
}



/*************************************************************
Config - setting
*************************************************************/
void Location::setLocationDirective( std::vector<std::string> line )
{
	this->_location = line[1];
}

void Location::setRoot( std::vector<std::string> line )
{
	if (_root == UNSET)
	{
		if (line.size() == 1)
		{
			_root = "";
			return ;
		}
		_root = line[1];
	}
	else
		throw std::invalid_argument("Error : Server block can't contain more than one root directive");
}

void Location::setAutoindex( std::vector<std::string> line )
{
   if (this->_set_autoindex == false)
	{
		if (line[1] == "off")
			_autoindex = false;
		else if (line[1] == "on")
			_autoindex = true;
		else
			throw std::invalid_argument("Error : autoindex directive is invalid");
		_set_autoindex = true;
	}
	else
		throw std::invalid_argument("Error : Server block can't contain more than one autoindex directive");
}

void Location::setMethods( std::vector<std::string> line )
{
	if (_methods[0] == UNSET)
	{
		if (line.size() == 1)
		{
			_methods[0] = "GET";
			return ;
		}
		_methods.clear();
		for (size_t i = 1; i < line.size(); i++)
			_methods.push_back(line[i]);
	}
	else
		throw std::invalid_argument("Error : location block can't contain more than one method directive");
}

void Location::setDefaultFile( std::vector<std::string> line )
{
	if (_default_file == UNSET)
	{
		_default_file=line[1];
	}
	else
		throw std::invalid_argument("Error : location block can't contain more than one index directive");
}

void Location::setUploadFolder( std::vector<std::string> line )
{
	if (_upload_folder == UNSET)
		_upload_folder = line[1];
	else
		throw std::invalid_argument("Error : location block can't contain more than one upload_folder directive");
}

void Location::setCgi( std::vector<std::string> line )
{
	std::string extension;
	std::string path;
	std::map<std::string, std::string>::iterator it = _cgi.begin();

	if (it->first == "0" && it->second == "0")
	{
		_cgi.clear();

		extension = line[1];
		path = line[2];

		_cgi.insert(std::pair<std::string, std::string>(extension, path));
	}
	else
		throw std::invalid_argument("Error : location block can't contain more than one cgi directive");
}

void Location::setRedirect( std::vector<std::string> line )
{
	if (_redirect == UNSET)
		_redirect = line[1];
	else
		throw std::invalid_argument("Error : location block can't contain more than one rewrite directive");
}

void Location::setUncalledDirectives()
{
	if (_root == UNSET) //Recuperer root du serveur si pas de root dans location
		_root = "";
	if (_methods[0] == UNSET)
		_methods[0] = "GET";
	if (_default_file == UNSET)
		_default_file = "";
		else
	if (_upload_folder == UNSET)
	{
		for (size_t i = 0; i < _methods.size(); i++)
		{
			if (_methods[i] == "POST")
				throw std::invalid_argument("Error : You must specify an upload directory if you use the POST method");
		}  
	}
	if (_redirect == UNSET)
		_redirect = "";
	//upload_folder reste = "-1" si pas de methode POST
	//redirect reste = "-1" si pas specifie
	//On ne change pas cgi (= 0, 0), comme ça on sait qu'il n'est pas demandé
}




/*************************************************************
Config - checking
*************************************************************/
bool Location::checkLocationData()
{
	setUncalledDirectives();

	checks _checks[CHECK_LOCATION_NB] = {&Location::checkLocation, &Location::checkRoot, 
			&Location::checkDefaultFile, &Location::checkMethods, &Location::checkUploadFolder, 
			&Location::checkCgi, &Location::checkRedirect};
	for (int i = 0; i < CHECK_LOCATION_NB; i++)
	{
		if ((this->*_checks[i])() == false)
			return false;
	}
//debug();
	return true;
}

bool Location::checkLocation()
{
	// un path '/' match toutes les requêtes
	if (_location.size() == 1 && _location[0] == '/')
		return true;
	//On accepte uniquement les char alnum et le '/', le '.', le '-' et le '_'
	if (!isAcceptableURI(_location))
	{
		throw std::invalid_argument("Error in location path : Invalid character.");
		return false;
	}
	return true;
}

bool Location::checkRoot()
{
	if (!isAcceptableURI(_root))
	{
		throw std::invalid_argument("Error in root directive : Invalid character.");
		return false;
	}
	return true;
}

bool Location::checkDefaultFile()
{
	if (!isAcceptableURI(_default_file))
	{
		throw std::invalid_argument("Error in index directive : Invalid character.");
		return false;
	}    
	return true;
}

bool Location::checkMethods()
{
	if (_methods.size() > 3)
		throw std::invalid_argument("Error in method directive : You can only call up to 3 methods.");
	
	int get = 0;
	int del = 0;
	int post = 0;

	for(size_t i = 0; i < _methods.size(); i++)
	{
		if (_methods[i] != "GET" && _methods[i] != "POST" && _methods[i] != "DELETE")
		{
			throw std::invalid_argument("Error in method directive : Invalid method.");
			return false;
		}
		if (_methods[i] == "GET")
			get++;
		else if (_methods[i] == "DELETE")
			del++;
		else if (_methods[i] == "POST")
			post++;
	}
	if (post > 1 || get > 1 || del > 1)
	{
		throw std::invalid_argument("Error in method directive : Methods can only be called once.");
		return false;
	} 
	if (get == 0)
		_methods.push_back("GET");
	return true;
}

bool Location::checkUploadFolder()
{
	if (_upload_folder == UNSET)
		return true;
	if (!isAcceptableURI(_upload_folder))
	{
		throw std::invalid_argument("Error in upload_folder directive : Invalid character.");
		return false;
	}
	return true;
}

bool Location::checkCgi()
{
	std::map<std::string, std::string>::iterator it;

	for (it = _cgi.begin(); it != _cgi.end(); it++)
	{
		if (it->first == "0" && it->second == "0")
			return true;
		if (isExtension(it->first) == false)
		{
			throw std::invalid_argument("Error in cgi directive : Invalid extension.");
			return false;
		}
		if (!isAcceptableURI(it->second))
		{
			throw std::invalid_argument("Error in CGI binary name : Invalid character.");
			return false;
		} 
	}
	return true;
}

bool Location::checkRedirect()
{
	if (_redirect == UNSET)
		return true;
	if (!isAcceptableURI(_redirect))
	{
		throw std::invalid_argument("Error in rewrite directive : Invalid character.");
		return false;
	} 

	return true;
}

bool Location::isEqual(const Location & rhs) const
{
	return _location == rhs.getLocation();
}




/*************************************************************
Getters
*************************************************************/
std::string Location::getLocation() const
{
	return this->_location;
}

std::string Location::getRoot() const
{
	return this->_root;
}

bool Location::getAutoIndex() const
{
	return this->_autoindex;
}

std::vector<std::string> Location::getMethods() const
{
	return this->_methods;
}

std::string Location::getDefaultFile() const
{
	return this->_default_file;
}

std::string Location::getUploadFolder() const
{
	return this->_upload_folder;
}

std::map<std::string, std::string> Location::getCgi() const
{
	return this->_cgi;
}

std::string Location::getCgiBinary() const
{
	 std::map<std::string, std::string>::const_iterator it = _cgi.begin();
	return it->second;
}

std::string Location::getCgiExtension() const
{
	std::map<std::string, std::string>::const_iterator it = _cgi.begin();
	return it->first;
}

std::string Location::getRedirect() const
{
	return this->_redirect;
}





void Location::debug()
{
	std::cout << "*** Debug location ***" << std::endl;
	std::cout << "Location : " << getLocation() << std::endl;
	std::cout << "Root : " << getRoot() << std::endl;
	std::cout << "autoindex - " << _autoindex << std::endl;
	for (size_t i = 0; i < _methods.size(); i++)
		std::cout << "method " << i << " - " <<  _methods[i] << std::endl;
	std::map<std::string, std::string>::iterator it;
	for (it = _cgi.begin(); it != _cgi.end(); it++)
		std::cout << "cgi - " << it->first << " " << it->second << std::endl;
	std::cout << "index " << " - " <<  _default_file << std::endl;
	std::cout << "Upload folder : " << getUploadFolder() << std::endl;
	std::cout << "Redirection " << _redirect << std::endl;
	std::cout << "*** End ***\n" << std::endl;
}









 bool Location::checkIfMethodIsPresent(std::string method) const
 {
	 std::vector<std::string> methods;
	 
	 methods = getMethods();

	std::vector<std::string>::iterator it_begin = methods.begin();
	std::vector<std::string>::iterator it_end = methods.end();

	for (int i = 0; it_begin != it_end; i++, it_begin++)
	{
		if (methods[i] == method)
			return (TRUE);
	}
	return (FALSE);
 }