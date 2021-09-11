#include "../includes/utils.hpp"
#include "Config.hpp"
#include "Request.hpp"
#include "Location.hpp"

void set_responseHTTP_get(Request &request, Config const & config, Location const & location);
void set_responseHTTP_according_to_method(Request &request, Config const & config, Location const & location)
{
    if (request.getMethod() == "GET")
    {
        set_responseHTTP_get(request, config, location);
    }
}

void set_responseHTTP_get(Request &request, Config const & config, Location const & location)
{
    (void) location;
    request.setResponseHTTPGet(config);
}