#include "VectorUp.hpp"
#include <iostream>
int main()
{
    VectorUp<int> test;

    test.push_back(5);
    test.push_back(4);
    
    int *fd;
    fd = test.data();
    
    std::cout << fd[0] << std::endl;
    std::cout << fd[1] << std::endl;
}