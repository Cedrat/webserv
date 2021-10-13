#include "../includes/utils.hpp"

void create_file(std::string path_name, std::string buffer)
{
    std::fstream fs;

    fs.open(path_name, std::fstream::in | std::fstream::app | std::fstream::binary);
    fs << buffer;
    fs.close();
}