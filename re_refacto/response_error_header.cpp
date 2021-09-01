#include "../includes/utils.hpp"
#include "Request.hpp"
#include "ResponseHTTP.hpp"

void response_header(Request request, fd fd_to_send)
{
    std::string response_header;
    std::string path = request.getResponseHTTP().getPath();


    response_header = "HTTP/1.1 " + get_string_error(request.getError()) + "\nContent-Length: " + int_to_string(get_file_size(path)) + "\n\n";
    std::cout << response_header << "RESPONSE" << std::endl;
    send(fd_to_send, response_header.c_str() , response_header.size() , 0);
}