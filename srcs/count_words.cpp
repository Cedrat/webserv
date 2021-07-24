#include "../includes/utils.hpp"

size_t count_words(std::string str)
{
    size_t count_word(0); 
    for (size_t i = 0; i <= str.size() ; i++)
    {
        while (str[i] != ' ' && i <= str.size())
        {
           i++;
        }
        while (str[i] == ' ' && i <= str.size())
            i++;
        count_word++;
    }
    return (count_word);
}