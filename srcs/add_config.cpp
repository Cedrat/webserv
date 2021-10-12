#include "ConfigParser.hpp"

void add_config(const char *path_file)
{
    try
    {
        ConfigParser cp(path_file);
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }

}