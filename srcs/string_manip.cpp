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

std::string int_to_string(int nb)
{
    std::string str;
    std::ostringstream temp;

    temp << nb;
    str = temp.str();
    return (str);
}

std::vector<std::string> split_string(std::string str, std::string splitter)
{
    std::vector<std::string> split_str;

    while (str.find(splitter) == 0)
        str.erase(0, splitter.size());
    std::cout << str << std::endl;
    while (str.size() > 0)
    {
        std::cout << str << std::endl;
        if (str.find(splitter) != std::string::npos)
        {
            split_str.push_back(str.substr(0, str.find(splitter)));
            str.erase(0, str.find(splitter) + splitter.size());
        }
        else
        {
            split_str.push_back(str.substr(0, str.size()));
            str.erase(0, str.size());
        }
        while (str.find(splitter) == 0)
            str.erase(0, splitter.size());        
    }
    return (split_str);
}

std::string    decoding_http_string(std::string str)
{
    while (str.find("%20") != std::string::npos)
    {
        str.replace(str.find("%20"), 3, " ", 1);
    }
    return(str);
}