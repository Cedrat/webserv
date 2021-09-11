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

std::string factorised_path(std::string path)
{
    std::vector<std::string> tokens;
    std::string new_path;
    size_t jump_next_path(0);

    tokens = split_string(path, "/");

    for (int i = tokens.size() - 1; i >= 0; i--)
    {
        if (tokens[i] == "..")
            jump_next_path++;
        else
        {
            if (jump_next_path == 0)
                new_path = "/" + tokens[i] + new_path;
            else
                jump_next_path--;
        }
    }
    if (path[path.size() - 1] == '/' || new_path == "")
        new_path += "/";
    new_path = decoding_http_string(new_path);
    return (new_path);
}
