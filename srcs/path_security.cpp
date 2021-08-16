#include "../includes/utils.hpp"

bool    check_valid_path(std::string path)
{
    std::vector<std::string> tokens;
    int possible = 0;

    tokens = split_string(path, "/");
    for (size_t i = 0; i < tokens.size(); i++)
    {
        if (tokens[i] == "..")
            possible -= 1;
        else
            possible += 1;
        if (possible < 0)
            return (FALSE);
    }
    return (TRUE);
}