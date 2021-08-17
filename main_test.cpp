#include "./includes/utils.hpp"

int main(int argc, char** argv)
{
    if (check_valid_path(argv[1]))
        std::cout << factorised_path(argv[1]) << std::endl;
    else
        std::cout << "Security breach detected" << std::endl;
}
