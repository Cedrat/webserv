#include <vector>
#include "define.hpp"

template<typename T>
bool search_in_vector(std::vector<T> vector, T const & value_to_found)
{
    for (size_t i = 0; i < vector.size() ; i++)
    {
        if (vector[i] == value_to_found)
            return (TRUE);
    }
    return (FALSE);
}