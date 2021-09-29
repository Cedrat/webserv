#include "AField.hpp"
#include "RequestInProgress.hpp"
#include "Location.hpp"
#include "Erreur.hpp"

AField::AField(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd) : _str_request(str_request), _data_request(data_request) , _pollfd(s_pollfd), _error(OK)
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

std::string const &AField::getQuery() const
{
    return (_query);
}
  
std::string const &AField::getTransfertEncoding() const
{
    std::cout << "bad call to getTransfertEncoding" << std::endl;
    throw "bad bad bad";
}

RequestInProgress const & AField::getDataRequest() const
{
    return (_data_request);
}

bool    AField::methodNotAuthorized(Location const & location) 
{
    return (!location.checkIfMethodIsPresent(_method));
}

void AField::verifyRedirect(Location const &location)
{
    if (location.getRedirect().empty() == FALSE)
        _error = MOVED_PERMANENTLY; 
}

AMethod *AField::createRedirMethod(Config config, Location location)
{
    std::string path_error = config.getPathError(_error);

    std::string header = "HTTP/1.1 " + get_string_error(_error);
    header += "\nLocation: " + redir_path(_path, location.getRedirect(), location.getLocation());    
    header += "\nContent-Length: " + int_to_string(get_file_size(path_error)) + "\n";
    header +=  date_string() + "\n\n";

    AMethod *method = new Erreur(_data_request.getFd(), path_error, header, *this);
    return (method);
}

void AField::setPollout()
{
   _pollfd.events = POLLOUT;
   _pollfd.revents = 0; 
}

void AField::setPollin()
{
   _pollfd.events = POLLIN;
   _pollfd.revents = 0; 
}