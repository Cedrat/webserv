#include "../includes/utils.hpp"

void delete_and_give_response(std::string path, fd current_fd)
{
    std::string page;

    page = "HTTP/1.1 " + get_string_error(204) + "\n\n";
    if (path[path.size() - 1 ] == '/')
        rmdir(path.c_str());
    if (remove(path.c_str()) == 0)
        send(current_fd, page.c_str(), page.size(), 0);
    else
        std::cout << "file not deleted" << std::endl;   
}