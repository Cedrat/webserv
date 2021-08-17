
#include "../includes/utils.hpp"

std::map<int, std::string> error_table()
{
    std::map<int, std::string> error_list;

    error_list[200] = "200 OK";
    error_list[204] = "204 No Content";
    error_list[400] = "400 Bad Request";
    error_list[404] = "404 Not Found";
    error_list[405] = "405 Method Not Allowed";
    error_list[505] = "505 Version not Supported";
    
    return (error_list);
}

std::string get_string_error(int num_error)
{
    std::map<int, std::string>::iterator it;

    return (error_table().find(num_error)->second);
}