#include "Location.hpp"

Location::Location()
{

}

Location::~Location()
{
    
}

void Location::setLocation(std::string location)
{
    _location = location;
}

void Location::setAutoIndex(bool autoindex_on)
{
    _autoindex = autoindex_on; 
}

void Location::setDefaultFile(std::string default_file)
{
    _default_file = default_file;
}

void Location::setUploadFolder(std::string upload_folder)
{
    _upload_folder = upload_folder;
}

void Location::addMethod(std::string method)
{
    _methods.push_back(method);
}

bool Location::getAutoIndex(void) const
{
    return(_autoindex);
}

std::string Location::getLocation(void) const
{
    return(_location);
}

void Location::setRoot(std::string root)
{
    _root = root;
}

std::string Location::getRoot(void)const
{
    return (_root);
}