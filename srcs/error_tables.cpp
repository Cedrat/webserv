
#include "../includes/utils.hpp"
#include <map>

std::map<int, std::string> error_table()
{
    std::map<int, std::string> error_list;

    error_list[200] = "200 OK";
    error_list[204] = "204 No Content";
    error_list[301] = "301 Moved Permanently";
    error_list[400] = "400 Bad Request";
    error_list[404] = "404 Not Found";
    error_list[405] = "405 Method Not Allowed";
    error_list[413] = "413 Request Entity Too Large";
    error_list[414] = "URI Too Long";
    error_list[418] = "418 I'm a teapot";
    error_list[501] = "501 Not Implemented";
    error_list[503] = "503 Service Unavailable";
    error_list[505] = "505 Version not Supported";
    
    return (error_list);
}

std::string get_string_error(int num_error)
{
    std::map<int, std::string>::iterator it;

    return (error_table().find(num_error)->second);
}

