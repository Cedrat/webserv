#include "../includes/utils.hpp"
#include "../utils/search_in_vector.hpp"

bool duplicata(std::string request)
{
    std::vector<std::string> fields_encountered;
    std::vector<std::string> request_lines;
    std::vector<std::string> actual_line;
    std::string              actual_field;

    request_lines = split_string(request, "\n");

    for (size_t i = 1; i < request_lines.size(); i++)
    {
        actual_line = split_string(request_lines[i], ":");
        actual_field = str_to_lower(actual_line[0]);
        if (search_in_vector(fields_encountered, actual_field))
            return (TRUE);
        else
            fields_encountered.push_back(actual_field);
    }
    return (FALSE);
}