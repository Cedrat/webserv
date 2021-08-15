#include "../includes/utils.hpp"

size_t nb_of_char_in_str(char character, std::string str)
{
    size_t nb_of_characters = 0;
    for (size_t i = 0; i < str.size() ; i++)
    {
        if (str[i] == character)
            nb_of_characters++;
    }
    return (nb_of_characters);
}