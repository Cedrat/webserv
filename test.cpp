#include <string>
#include <iostream>

int main()
{
    std::string *_deux;
    _deux = nullptr;
    if (_deux)
        std::cout << "Coucou " << std::endl;
    if (_deux == nullptr)
        std::cout << "Absent" << std::endl;

    _deux = new std::string("un");
    if (_deux)
        std::cout << "Coucou " << std::endl;
    if (_deux == nullptr)
        std::cout << "Absent" << std::endl;
    
    delete _deux;
    _deux = nullptr;

    if (_deux)
        std::cout << "Coucou " << std::endl;
    if (_deux == nullptr)
        std::cout << "Absent" << std::endl;
}