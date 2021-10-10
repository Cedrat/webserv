#include "../includes/utils.hpp"
#include "Location.hpp"

std::string  fields_allowed(Location location)
{
    std::string allow_field = "Allow: ";

    for (size_t i = 0; i < location.getMethods().size(); i++)
    {
        allow_field += location.getMethods()[i];
        if ((i + 1) != location.getMethods().size())
            allow_field += ", ";
    }
    allow_field += "\n";
    return allow_field;
}
