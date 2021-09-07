#include "../includes/utils.hpp"
#include "Request.hpp"
#include "ResponseHTTP.hpp"

void response_header(Request request, fd fd_to_send)
{
    std::string response_header;
    std::string path = request.getResponseHTTP().getPath();
    if (request.getResponseHTTP().getPageAutoIndex() != "")
        response_header = "HTTP/1.1 " + get_string_error(request.getError()) + "\nContent-Length: " + int_to_string(request.getResponseHTTP().getPageAutoIndex().size()) + "\n\n";
    else if (request.getError() != MOVED_PERMANENTLY)
        response_header = "HTTP/1.1 " + get_string_error(request.getError()) + "\nContent-Length: " + int_to_string((int)get_file_size(path)) + "\n\n";
    else
    {
        response_header = "HTTP/1.1 " + get_string_error(request.getError()) + "\nLocation: " + request.getPath() + "\nContent-Length: " + int_to_string((int)get_file_size(path)) + "\n\n";
    }
    
    
    std::cout << "how much byte ? " << send(fd_to_send, response_header.c_str() , response_header.size() , 0) << std::endl;
}