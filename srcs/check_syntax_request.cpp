#include "../includes/utils.hpp"
#include "define.hpp"


int    check_syntax_request(std::string str_request)
{
    std::vector<std::string> all_lines;
    char motif_method[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/(1\\.0|1\\.1)\r\n$";
    char motif_version_not_supported[] = "^[A-Z]+ +\\/[!-~]* +HTTP\\/[23]\r\n$";
    char motif2[] = "[a-zA-z0-9]+: +.*[^ ]+ *\r\n";
    //std::cout << "_______" << str_request << "_______" << std::endl;
    all_lines = split_string(str_request, "\n");
    if (str_request != "")
    {
        if (match_regex(const_cast<char *>((all_lines[0] + "\n").c_str()), motif_version_not_supported) >= 1)
            return(NOT_SUPPORTED);
        else if (match_regex(const_cast<char *>((all_lines[0] + "\n").c_str()), motif_method) < 1)
            return(BAD_REQUEST);
    }
    if ( str_request != "")
    {
        for (size_t i = 1; i < all_lines.size(); i++)
        {
            if (match_regex(const_cast<char *>((all_lines[i] + "\n").c_str()), motif2) >= 1)
            {
            }
            else if (i == (all_lines.size()-1) && all_lines[i] == "\r")
            {

            }
            else 
            {
                return(BAD_REQUEST);
            }
        }
    }
    return (OK);
}
