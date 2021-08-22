#include "../includes/utils.hpp"

void response_post(int num_code,  Config config, fd fd_to_answer)
{
    std::string page("Created with success");
    
    (void) config;
    (void) num_code;
    page = "HTTP/1.1 " + get_string_error(201) +"\nContent-Length: " + int_to_string(page.size()) + "\n\n" + page;
    send(fd_to_answer, page.c_str(), page.size(), 0);
}