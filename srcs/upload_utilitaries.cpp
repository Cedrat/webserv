#include "../includes/utils.hpp"

static std::string extract_extension_in_content_disposition(std::string str_to_process)
{
    std::string filename = split_string(str_to_process, "filename=")[1];

    trim(filename, '\"');

    std::vector<std::string> decomposed_filename = split_string(filename, ".");
    return (decomposed_filename[decomposed_filename.size() -1]);
}

std::string extract_extension(std::string body)
{
    std::vector<std::string> splitted_request = split_string(body, "\r\n");
    std::vector<std::string> splitted_line;


    for (size_t i = 0; i < splitted_request.size(); i++)
    {
        splitted_line = split_string(splitted_request[i], ": ");
        if (str_to_lower(splitted_line[0]) == "content-disposition")
        {
            return (extract_extension_in_content_disposition(splitted_line[1]));
        }

    }
    return ("");
}