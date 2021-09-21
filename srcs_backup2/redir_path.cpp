#include "../includes/utils.hpp"

std::string redir_path(std::string path, std::string path_redir, std::string part_to_replace)
{
    path.replace(path.find(part_to_replace), part_to_replace.size(), path_redir);

    return (path);
}
