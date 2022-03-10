#include "../includes/utils.hpp"
#include "../config/Config.hpp"

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
    std::vector<std::string> splitted_path = split_string(path, "/");
    std::vector<std::string> splitted_location;
    size_t nb_of_precision(0);
    size_t old_nb_of_precision(0);


    for (size_t i = 0; i < locations.size(); i++)
    {
        splitted_location = split_string(locations[i].getLocation(), "/");
        nb_of_precision = 1;
        for (size_t p = 0; p < splitted_path.size() && p < splitted_location.size(); p++)
        {
            if (splitted_path[p] == splitted_location[p])
                nb_of_precision++;
            else 
                p = splitted_path.size();
        }
        if (nb_of_precision > old_nb_of_precision)
        {
            old_nb_of_precision = nb_of_precision;
            best_location = locations[i];
        }
    }
    return (best_location);
}
/*
Pour trouver la meilleure location :
utiliser le chemin auquel l'utilisateur veut avoir accés
    split le chemin de l'utilisateur par rapport aux "/" // exemple /test/direct/index.html -> test direct index.html

    split les locations par rapport aux "/" aussi // /test/ -> test

    regarder le nombre de correspondance d'AFFILES 
    
    /test/direct/ -> test direct sera plus précis que /test/

    /test/
    /direct/

*/
