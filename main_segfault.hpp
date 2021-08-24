#include <iostream>

class DoubleInt
{
    public :
    int first;
    int second;

    DoubleInt()
    {
        first = 1;
        first = 2;
        std::cout << "Constructor called" << std::endl;
        }
        ~DoubleInt()
        {
            std::cout << "Destructor called " << std::endl;
        }
};