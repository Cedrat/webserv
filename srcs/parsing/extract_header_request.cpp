#include "../includes/utils.hpp"

std::string extract_header_request(std::string request)
{
    return (split_string(request, "\r\n\r\n")[0] + "\r\n");
}