#include "../includes/utils.hpp"
#include "Request.hpp"
#include "VerifyDuplicata.hpp"

std::string extract_method(std::string request);
std::string extract_path(std::string request);
std::string extract_host_name(std::string request);
int extract_content_length(std::string request);

void process_data(Request &request, std::vector<Config> configs)
{
    std::string str_request = request.getRequest();
    VerifyDuplicata verify(str_request);
    if (verify.isDuplicata() == TRUE)
    {
        request.setError(BAD_REQUEST);
        return;
    }
    request.setMethod(extract_method(str_request));
    std::cout << "Method = " << request.getMethod() << std::endl;
    request.setPath(extract_path(str_request));
    std::cout << "Path = " << request.getPath() << std::endl;
    request.setHostName(extract_host_name(str_request));
    std::cout << "Host-Name = " << request.getHostName() << std::endl;
    if (verify.getNbContentLength() == 1)
        request.setContentLength(extract_content_length(str_request));
    std::cout << "Content_length = " << request.getContentLength() << std::endl;
}

std::string extract_method(std::string request)
{
    std::string method;

    method = request.substr(0, request.find(" "));

    return (method);
}

std::string extract_path(std::string request)
{
    std::string path;

    path = request.substr(request.find(" ") + 1, request.find("HTTP") - request.find(" ") - 2);

    return (path);
}

std::string extract_host_name(std::string request)
{
    std::string host_name;

    request.erase(0, request.find("Host"));
    request.erase(request.find("\n"));
    host_name = request.substr(request.rfind(" ") + 1);
    return (host_name);
}

int extract_content_length(std::string request)
{
    int content_length;

    request.erase(0, request.find("Content-Length"));
    request.erase(request.find("\n"));
    content_length = string_to_int(request.substr(request.rfind(" ") + 1));
    return (content_length);
}