#include "./includes/utils.hpp"

int main()
{
    std::cout << check_valid_path("/../../deux/../../trois/") << std::endl;
    std::cout << check_valid_path("/deux/../trois/") << std::endl;
    std::cout << check_valid_path("/../trois/") << std::endl;
}