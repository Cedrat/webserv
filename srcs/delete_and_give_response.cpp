#include "../includes/utils.hpp"

void delete_and_give_response(std::string path, fd current_fd)
{
    std::string page;

    std::cout << "path to delete = " << path << std::endl;
    page = "HTTP/1.1 " + get_string_error(204) + "\n\n";
    delete_f(path.c_str());
    send(current_fd, page.c_str(), page.size(), 0);
}