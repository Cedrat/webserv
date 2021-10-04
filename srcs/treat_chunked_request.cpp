#include "../includes/utils.hpp"
#include <cmath>

std::string process_chunked_request(std::string request)
{
    std::string str_nb_bytes_next;
    std::string processed_chunk;
    int         nb_bytes_next;

    while (request.find("\r\n") == 0)
    {
        request.erase(0, 2);
        str_nb_bytes_next = request.substr(0,request.find("\r\n"));
        request.erase(0, request.find("\r\n") + 2);
        
        nb_bytes_next = atoi_hexa(str_nb_bytes_next);
        processed_chunk += request.substr(0, nb_bytes_next);
        request.erase(0, nb_bytes_next);
    }
    return processed_chunk;
}