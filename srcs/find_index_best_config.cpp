#include "../includes/utils.hpp"
#include "Config.hpp"

bool find_host_name(std::vector<std::string> hosts_names, std::string host_name)
{
    for (size_t i = 0; i < hosts_names.size() ; i++)
    {
        if (hosts_names[i] == host_name)
            return (TRUE);
    }
    return (FALSE);
}

size_t find_index_best_config(std::vector<Config> configs, std::string host_name, size_t port, int host)
{
    for (size_t i = 0; i < configs.size(); i++)
    {
        if (configs[i].getPort() == port && configs[i].getHost() == host)
        {
            if (find_host_name(configs[i].getServerNames(), host_name))
                return (i);
        }
    }
    for (size_t i = 0; i < configs.size(); i++)
    {
        if (configs[i].getPort() == port && configs[i].getHost() == host)
        {
            if (configs[i].IsPrincipalServer() == TRUE)
                return(i);
        }
    }
    throw ("no config corresponding");
}

Location find_best_location(std::string path, Config config)
{
    std::vector<Location> locations = config.getLocations();
    Location best_location;
    size_t     nb_of_precision(0);
    for (size_t i = 0; i < locations.size(); i++)
   {
       if ((path.find(locations[i].getLocation()) != std::string::npos) && nb_of_char_in_str('/', locations[i].getLocation()) > nb_of_precision)
       {
            best_location = locations[i];
            nb_of_precision = nb_of_char_in_str('/', locations[i].getLocation());
            if (locations[i].getLocation() == "/")
                nb_of_precision = 0;
       }
   }
   std::cout << "Best location is " << best_location.getLocation() << std::endl;
   return (best_location);
}
