#include "ConfigParser.hpp"

int main()
{
    char file[] = "./step.conf";
    try
    {
        ConfigParser cp(file);
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}