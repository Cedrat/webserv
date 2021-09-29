#include <string>
#include <iostream>

int main()
{
    std::string *_deux;
    _deux = NULL;
    if (_deux)
        std::cout << "Coucou " << std::endl;
    if (_deux == NULL)
        std::cout << "Absent" << std::endl;

    _deux = new std::string("un");
    if (_deux)
        std::cout << "Coucou " << std::endl;
    if (_deux == NULL)
        std::cout << "Absent" << std::endl;
    
    delete _deux;
    _deux = NULL;

    if (_deux)
        std::cout << "Coucou " << std::endl;
    if (_deux == NULL)
        std::cout << "Absent" << std::endl;
}