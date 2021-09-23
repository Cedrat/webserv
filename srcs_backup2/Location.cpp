#include "Location.hpp"

Location::Location() : _autoindex(FALSE)
{
    this->_root = "-1";
    this->_methods.push_back("-1");
    this->_set_autoindex = false;
    this->_default_file = "-1";
    this->_upload_folder = "-1";
    this->_cgi.insert(std::pair<std::string, std::string>("0", "0"));
    this->_redirect = "-1";
}

Location::~Location() { }





/*************************************************************
Config - setting
*************************************************************/
void Location::setLocationDirective( std::vector<std::string> line )
{
    this->_location = line[1];
}

void Location::setRoot( std::vector<std::string> line )
{
    if (_root == "-1")
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
    if (_methods[0] == "-1")
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
    if (_default_file == "-1")
    {
        _default_file=line[1];
    }
    else
        throw std::invalid_argument("Error : location block can't contain more than one index directive");
}

void Location::setUploadFolder( std::vector<std::string> line )
{
    if (_upload_folder == "-1")
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
    if (_redirect == "-1")
        _redirect = line[1];
    else
        throw std::invalid_argument("Error : location block can't contain more than one rewrite directive");
}

void Location::setUncalledDirectives()
{
    if (_root == "-1") //Recuperer root du serveur si pas de root dans location
        _root = "";
    if (_methods[0] == "-1")
        _methods[0] = "GET";
    if (_default_file == "-1")
        _default_file = "";
        else
    if (_upload_folder == "-1")
    {
        for (size_t i = 0; i < _methods.size(); i++)
        {
            if (_methods[i] == "POST")
                throw std::invalid_argument("Error : You must specify an upload directory if you use the POST method");
        }  
    }
    if (_redirect == "-1")
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

    checks _checks[7] = {&Location::checkLocation, &Location::checkRoot, 
            &Location::checkDefaultFile, &Location::checkMethods, &Location::checkUploadFolder, 
            &Location::checkCgi, &Location::checkRedirect};
    for (int i = 0; i < 7; i++)
    {
        if ((this->*_checks[i])() == false)
            return false;
    }
debug();
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
        std::cerr << "Error in location path : Invalid character" << std::endl;
        return false;
    }
    return true;
}

bool Location::checkRoot()
{
    if (!isAcceptableURI(_root))
    {
        std::cerr << "Error in root directive : Invalid character" << std::endl;
        return false;
    }
    return true;
}

bool Location::checkDefaultFile()
{
    if (!isAcceptableURI(_default_file))
    {
        std::cerr << "Error in index directive : Invalid character" << std::endl;
        return false;
    }    
    return true;
}

bool Location::checkMethods()
{
    if (_methods.size() > 3)
        return false;
    
    int get = 0;
    int del = 0;
    int post = 0;

    for(size_t i = 0; i < _methods.size(); i++)
    {
        if (_methods[i] != "GET" && _methods[i] != "POST" && _methods[i] != "DELETE")
            return false;
        if (_methods[i] == "GET")
            get++;
        else if (_methods[i] == "DELETE")
            del++;
        else if (_methods[i] == "POST")
            post++;
    }
    if (post > 1 || get > 1 || del > 1)
        return false;
    if (get == 0)
        _methods.push_back("GET");
    return true;
}

bool Location::checkUploadFolder()
{
    if (_upload_folder == "-1")
        return true;
    if (!isAcceptableURI(_upload_folder))
    {
        std::cerr << "Error in upload_folder directive : Invalid character" << std::endl;
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
            std::cerr << "Error in cgi directive : Invalid extension" << std::endl;
            return false;
        }
        if (!isAcceptableURI(it->second))
        {
            std::cerr << "Error in CGI binary name : Invalid character" << std::endl;
            return false;
        } 
    }
    return true;
}

bool Location::checkRedirect()
{
    if (_redirect == "-1")
        return true;
    if (!isAcceptableURI(_redirect))
    {
        std::cerr << "Error in rewrite directive : Invalid character" << std::endl;
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