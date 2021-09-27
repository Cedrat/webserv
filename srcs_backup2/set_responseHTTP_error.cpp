#include "Request.hpp"
#include "ResponseHTTP.hpp"
#include "Config.hpp"

void set_responseHTTP_error(Request &request, std::vector<Config> configs)
{
   size_t index_best_config = find_index_best_config(configs, request.getHostName(), request.getPort(), request.getHost());

    request.setResponseHTTPError(configs[index_best_config]);
    
}