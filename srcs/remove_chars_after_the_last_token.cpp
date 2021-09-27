#include "../includes/utils.hpp"

std::string remove_chars_after_the_last_token(std::string str, char token)
{
    std::string::iterator it_begin = str.begin();
    for (int i = str.size(); i >= 0; i--)
    {
        if (str[i] == token)
            return str;
        str.erase(i + it_begin);
    }
    return (str);
}