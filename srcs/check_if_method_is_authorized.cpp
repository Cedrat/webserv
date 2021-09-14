#include "../includes/utils.hpp"
#include "Config.hpp"
#include "Request.hpp"

Location find_best_location(Request request, Config config)
{
    std::vector<Location> locations = config.getLocations();
    Location best_location;
    size_t     nb_of_precision(0);
    for (size_t i = 0; i < locations.size(); i++)
   {
       std::cout << "AI" << request.getPath() << locations[i].getLocation() << std::endl;
       if ((request.getPath().find(locations[i].getLocation()) != std::string::npos) && nb_of_char_in_str('/', locations[i].getLocation()) > nb_of_precision)
       {
            best_location = locations[i];
            nb_of_precision = nb_of_char_in_str('/', locations[i].getLocation());
       }
   }
   return (best_location);
}

bool check_if_method_is_authorized(Request &request, Config config)
{
    std::vector<std::string> methods = find_best_location(request, config).getMethods();

    for (size_t i = 0; i < methods.size() ; i++)
    {
       if (methods[i] == request.getMethod())
        return (TRUE); 
    }
    return (FALSE);
}