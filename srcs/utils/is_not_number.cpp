#include "../includes/utils.hpp"

bool char_is_not_number(char c)
{
    if (c < '0' || '9' < c)
        return (TRUE);
    return (FALSE);
}

bool str_is_not_number(std::string str)
{
    for (size_t i = 0;  i < str.size(); i++)
    {
        if (char_is_not_number(str[i]))
        {
            return (TRUE);
        }
    }
    return (FALSE);
}