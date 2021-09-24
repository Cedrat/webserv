#include "../includes/utils.hpp"

void trim_field(std::string &str_to_trim)
{
    trim(str_to_trim, '\r');
    trim(str_to_trim, ' ');
}