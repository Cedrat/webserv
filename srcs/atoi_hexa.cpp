#include "../includes/utils.hpp"
#include <cmath>

static int pos_in_hexa_array(char value)
{
    char hexa_array[] = "0123456789ABCDEF";

    value = toupper(value);

    for (int i = 0; i < 17; i++)
    {
        if (hexa_array[i] == value)
            return (i);
    }
    throw "bad hexa value";
}
int atoi_hexa(std::string nb)
{
    size_t decimal_value = 0;

    for (size_t i = 0; i < nb.size(); i++)
    {
        decimal_value += pos_in_hexa_array(nb[i]) * std::pow(16, nb.size() - (i + 1));
    }    return decimal_value;
}
