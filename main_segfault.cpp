#include <vector>

#include "main_segfault.hpp"


void create(std::vector<DoubleInt> &ref)
{
    DoubleInt  * Bises = new DoubleInt;

    ref.push_back(*Bises);
}

int main()
{
    std::vector<DoubleInt> more_int;
    DoubleInt  * Bises = new DoubleInt;
    
    // create(more_int);
    // create(more_int);
    // create(more_int);
    // create(more_int);
    // create(more_int);
    // create(more_int);
    more_int.push_back(*Bises);
    std::cout << more_int[0].first << std::endl;

    for (int i = 0; i < more_int.size(); i++)
    {
       std::cout << &more_int[i] << std::endl;
    }

}