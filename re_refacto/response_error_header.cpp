#include "../includes/utils.hpp"
#include "Request.hpp"
#include "ResponseHTTP.hpp"

void response_header(Request request, fd fd_to_send)
{
    std::string response_header;
    std::string path = request.getResponseHTTP().getPath();

    response_header = "HTTP/1.1 " + get_string_error(request.getError()) + "\r\nContent-Length: " + int_to_string((int)get_file_size(path)) + "\r\n\r\n";
    
    std::cout << "how much byte ? " << send(fd_to_send, response_header.c_str() , response_header.size() , 0) << std::endl;
}