#include "Location.hpp"

Location::Location() : _autoindex(FALSE)
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

std::string Location::getDefaultFile(void) const
{
    return (_default_file);
}

void Location::setRedirect(std::string redirect)
{
    _redirect = redirect;
}

std::string Location::getRedirect() const
{
    return (_redirect);
}

 std::vector<std::string>    Location::getMethods(void) const
 {
     return (_methods);
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