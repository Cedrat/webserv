#include "../includes/utils.hpp"

void response_header(int num_error, fd fd_to_send)
{
    std::string response_header;

    response_header = "HTTP/1.1 " + get_string_error(num_error) + "\n";
    send(fd_to_send, response_header.c_str() , response_header.size() , 0); 
}