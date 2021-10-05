#include "../includes/utils.hpp"

template<typename T>
std::vector<T> copy_value_of_pointer_vector(std::vector<T*>  const & input_vector)
{
    std::vector<T> output_vector(input_vector.size());

    for (size_t i = 0; i < input_vector.size(); i++)
    {
        output_vector[i] = *input_vector[i];
    }
    return output_vector;
}

template<typename T>
std::vector<T*> copy_value_in_pointer_vector(std::vector<T>  const & input_vector)
{
    std::vector<T*> output_vector(input_vector.size());

    for (size_t i = 0; i < input_vector.size(); i++)
    {
        output_vector[i] = input_vector[i];
    }
    return output_vector;
}
