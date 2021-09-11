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
            if (find_host_name(configs[i].getServersNames(), host_name))
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