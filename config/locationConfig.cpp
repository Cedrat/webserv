#include "locationConfig.hpp"

locationConfig::locationConfig()
{
    this->_root = "-1";
    this->_autoindex = false;
    this->_methods.push_back("-1");
    this->_index.push_back("-1");
    this->_upload_folder = "-1";
    this->_cgi.insert(std::pair<std::string, std::string>("0", "0"));
}

locationConfig::~locationConfig() { }





/*************************************************************
Config - setting
*************************************************************/
void locationConfig::setLocationDirective( std::vector<std::string> line )
{
    this->_location = line[1];
}

void locationConfig::setRoot( std::vector<std::string> line )
{
    if (_root == "-1")
    {
        if (line.size() == 1)
        {
            _root = "./";
            return ;
        }
        _root = line[1];
    }
    else
        throw std::invalid_argument("Error : Server block can't contain more than one root directive");
}

void locationConfig::setAutoindex( std::vector<std::string> line )
{
    //Ajouter système pour check qu'autoindex n'est pas call plusieurs fois
    if (line[1] == "off")
        _autoindex = false;
    else if (line[1] == "on")
        _autoindex = true;
    else
        throw std::invalid_argument("Error : autoindex directive is invalid");
}

void locationConfig::setMethods( std::vector<std::string> line )
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

void locationConfig::setIndex( std::vector<std::string> line )
{
    if (_index[0] == "-1")
    {
        if (line.size() == 1)
        {
            _index[0] = "../www/tester/index.html";
            return ;
        }
        _index.clear();
        for (size_t i = 1; i < line.size(); i++)
            _index.push_back(line[i]);
    }
    else
        throw std::invalid_argument("Error : location block can't contain more than one index directive");
}

void locationConfig::setUploadFolder( std::vector<std::string> line )
{
    if (_upload_folder == "-1")
    {
        if (line.size() == 1)
        {
            _upload_folder = "./www/post_test";
            return ;
        }
        _upload_folder = line[1];
    }
    else
        throw std::invalid_argument("Error : location block can't contain more than one upload_folder directive");
}

void locationConfig::setCgi( std::vector<std::string> line )
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

void locationConfig::setUncalledDirectives()
{
    if (_root == "-1")
        _root = "./";
    if (_methods[0] == "-1")
        _methods[0] = "GET";
    if (_index[0] == "-1")
    {
        if (_root[_root.size() - 1] == '/')
            _index[0] = "index.html";
        else
            _index[0] = "/index.html";
    }
    if (_upload_folder == "-1")
    {
        for (size_t i = 0; i < _methods.size(); i++)
        {
            if (_methods[i] == "POST")
                throw std::invalid_argument("Error : You must specify an upload directory if you use the POST method");
        }
        _upload_folder = "../www/post_test";
    }
    //On ne change pas cgi (= 0, 0), comme ça on sait qu'il n'est pas nécessaire
}




/*************************************************************
Config - checking
*************************************************************/
bool locationConfig::checkLocationData()
{
    setUncalledDirectives();
    for (int i = 0; i < 6; i++)
    {
        if ((this->*_checks[i])() == false)
            return false;
    }
debug();
    return true;
}

bool locationConfig::checkLocation()
{
    // un path '/' match toutes les requêtes
    if (_location.size() == 1 && _location[0] == '/')
        return true;
    //On accepte uniquement les char alnum et le '/'
    for (size_t i = 0; i < _location.size(); i++)
    {
        if (!isalnum(_location[i]) && _location[i] != '/')
        {
            std::cerr << "Error in location path : Invalid character" << std::endl;
            return false;
        }
    }
    return true;
}

bool locationConfig::checkRoot()
{
    struct stat state{};

    stat(_root.c_str(), &state);
    if (!S_ISDIR(state.st_mode))
    {
        std::cerr << "Error in root directive" << std::endl;
        return false;
    }
    return true;
}

bool locationConfig::checkIndex()
{
    std::ifstream ifs;
    std::string path = _root;

    for (size_t i = 0; i < _index.size(); i++)
    {
        if (path[path.size() - 1] == '/' && _index[i][0] == '/')
            path = _root.substr(0, _root.size() - 1);
        if (path[path.size() - 1] != '/' && _index[i][0] != '/')
            path += '/';
        path += _index[i];
        ifs.open(path.c_str(), std::ifstream::in);
        if (ifs.is_open() == false)
        {
            ifs.close();
            std::cerr << "Error in index directive" << std::endl;
            return false;
        } 
    }
    return true;
}

bool locationConfig::checkMethods()
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
    return true;
}

bool locationConfig::checkUploadFolder()
{
    struct stat state;

    stat(_upload_folder.c_str(), &state);
    if (!S_ISDIR(state.st_mode))
    {
        std::cerr << "Error in upload_folder directive" << std::endl;
        return false;
    }
    return true;
}

bool locationConfig::checkCgi()
{
    std::map<std::string, std::string>::iterator it;

    for (it = _cgi.begin(); it != _cgi.end(); it++)
    {
        if (it->first == "0" && it->second == "0")
            return true;
        if (isExtension(it->first) == false)
        {
            //throw std::invalid_argument("Error in cgi directive");
            std::cerr << "Error in cgi directive" << std::endl;
            return false;
        }   

        std::ifstream ifs;
        ifs.open(it->second.c_str(), std::ifstream::in);
        if (ifs.is_open() == false)
        {
            ifs.close();
            std::cerr << "Error in cgi directive" << std::endl;
            return false;
        }    
    }
    return true;
}



/*************************************************************
Getters
*************************************************************/
std::string locationConfig::getLocation() const
{
    return this->_location;
}

std::string locationConfig::getRoot() const
{
    return this->_root;
}

bool locationConfig::getAutoindex() const
{
    return this->_autoindex;
}

std::vector<std::string> locationConfig::getMethods() const
{
    return this->_methods;
}

std::vector<std::string> locationConfig::getIndex() const
{
    return this->_index;
}

std::string locationConfig::getUploadFolder() const
{
    return this->_upload_folder;
}

std::map<std::string, std::string> locationConfig::getCgi() const
{
    return this->_cgi;
}





void locationConfig::debug()
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
    for (size_t i = 0; i < _index.size(); i++)
        std::cout << "index " << i << " - " <<  _index[i] << std::endl;
    std::cout << "Upload folder : " << getUploadFolder() << std::endl;
    std::cout << "*** End ***\n" << std::endl;
}