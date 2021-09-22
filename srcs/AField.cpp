#include "AField.hpp"
#include "RequestInProgress.hpp"
#include "Location.hpp"

AField::AField(std::string str_request, RequestInProgress data_request) : _str_request(str_request), _data_request(data_request) , _error(OK)
{
}

AField::~AField()
{

}

std::string const &AField::getMethod() const
{
    return (_method);
}

std::string const &AField::getPath() const
{   
    return (_path); 
}

std::string const &AField::getHostName() const
{
    return (_host_name);
}

std::string const &AField::getStrRequest() const
{
    return (_str_request);
}

std::string const &AField::getHeader() const
{
    return (_header);
}

RequestInProgress const & AField::getDataRequest() const
{
    return (_data_request);
}

bool    AField::methodNotAuthorized(Location const & location) 
{
    return (!location.checkIfMethodIsPresent(_method));
}