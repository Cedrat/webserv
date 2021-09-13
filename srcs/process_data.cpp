#include "../includes/utils.hpp"
#include "Request.hpp"
#include "VerifyDuplicata.hpp"
#include "Config.hpp"
#include "ResponseHTTP.hpp"
#include "search_in_vector.hpp"

std::string extract_method(std::string request);
std::string extract_path(std::string request);
std::string extract_host_name(std::string request);
int extract_content_length(std::string request);
bool duplicata(std::string request);
bool not_enough_field(std::string method, std::string request);
int verif_post_data(Request & request, std::vector<Config> configs);

void process_data(Request &request, std::vector<Config> configs)
{
    //the syntax is verified below, after retrieve data
    std::string method;
    Config      best_config;
    Location    best_location;

    request.setPath(extract_path(request.getRequest()));
    method = extract_method(request.getRequest());

    if (duplicata(request.getRequest()) 
    || not_enough_field(method, request.getRequest())
    || (check_valid_path(request.getPath()) == FALSE && request.getMethod() != "POST"))
    {
        std::cout << "POST BAD REQUEST" << std::endl;
        return(request.setError(BAD_REQUEST));
    }
    request.setMethod(method);
    request.setPath(factorised_path(request.getPath()));
    request.setHostName(extract_host_name(request.getRequest()));
    
    best_config = configs[find_index_best_config(configs, 
            request.getHostName(), request.getPort(), request.getHost())];
    best_location = find_best_location(request, best_config);
    if (check_if_method_is_authorized(request, best_config) == FALSE)
    {
        return (request.setError(METHOD_NOT_ALLOWED));
    }
    //request.setPath(construct_path(request.getPath(), best_location));
    if (request.getMethod() == "POST")
    {
        if (verif_post_data(request, configs) < 0)
        {
            return ;
        }
    set_responseHTTP_according_to_method(request, best_config, best_location);
    }
}

int verif_post_data(Request & request, std::vector<Config> configs)
{
    int content_length(-1);
    std::string transfert_encoding;
    Config best_config = configs[find_index_best_config(configs,request.getHostName(), request.getPort(), request.getHost())];

    bool host(0);

    std::vector<std::string> request_lines;
    std::vector<std::string> actual_line;
    std::string              actual_field;

    request_lines = split_string(request.getRequest(), "\n");
    for (size_t i = 1; i < request_lines.size(); i++)
    {
        actual_line = split_string(request_lines[i], ":");
        actual_field = str_to_lower(actual_line[0]);
        if (actual_field == "host")
            host = 1;
        else if (actual_field == "content-length")
            content_length = atoi(actual_line[1].c_str());
        else if (actual_field == "transfert-encoding")
        {
            trim(actual_line[1], ' ');
            transfert_encoding = actual_line[1];
        }
    }
    if ((transfert_encoding == "chunked" && content_length > 0) 
    || (transfert_encoding != "chunked" && content_length == -1)
    || (host == 0)
    || (best_config.getMaxBodySize() < content_length))
    {
        request.setError(BAD_REQUEST);
        std::cout << "POST BAD " << std::endl;
        return (-1);
    }

    return (1);
}

bool not_enough_field(std::string method, std::string request)
{
    std::vector<std::string> request_lines;
    std::vector<std::string> actual_line;
    std::string              actual_field;
    int                      verif_number(0);

    request_lines = split_string(request, "\n");
    if (method == "GET" || method == "DELETE")
    {
         for (size_t i = 1; i < request_lines.size(); i++)
        {
            actual_line = split_string(request_lines[i], ":");
            actual_field = str_to_lower(actual_line[0]);
            if (actual_field == "host")
                return (FALSE);
        }
    }
    else if (method == "POST")
    {
         for (size_t i = 1; i < request_lines.size(); i++)
        {
            actual_line = split_string(request_lines[i], ":");
            actual_field = str_to_lower(actual_line[0]);
            std::cout << "line -> " << actual_field << std::endl;
            if (actual_field == "content-length" || actual_field == "transfer-encoding" || actual_field == "host")
                verif_number++;
        }
        if (verif_number >= 2)
            return (FALSE);
    }
    return (TRUE);
}

bool duplicata(std::string request)
{
    std::vector<std::string> fields_encountered;
    std::vector<std::string> request_lines;
    std::vector<std::string> actual_line;
    std::string              actual_field;

    request_lines = split_string(request, "\n");

    for (size_t i = 1; i < request_lines.size(); i++)
    {
        actual_line = split_string(request_lines[i], ":");
        actual_field = str_to_lower(actual_line[0]);
        if (search_in_vector(fields_encountered, actual_field))
            return (TRUE);
        else
            fields_encountered.push_back(actual_field);
    }
    return (FALSE);
}

std::string extract_method(std::string request)
{
    std::string method;
    std::vector<std::string> request_splitted;

    request_splitted = split_string(request, " ");

    return (request_splitted[0]);
}

std::string extract_path(std::string request)
{
    std::string method;
    std::vector<std::string> request_splitted;

    request_splitted = split_string(request, " ");

    return (request_splitted[1]);
}

std::string extract_host_name(std::string request)
{
    std::vector<std::string> fields_encountered;
    std::vector<std::string> request_lines;
    std::vector<std::string> actual_line;
    std::string              actual_field;

    request_lines = split_string(request, "\n");

    for (size_t i = 1; i < request_lines.size(); i++)
    {
        actual_line = split_string(request_lines[i], " ");
        actual_field = str_to_lower(actual_line[0]);
        if (actual_field == "host")
            return (actual_line[1]);
    }
    return "";
}

int extract_content_length(std::string request)
{
    int content_length;

    request.erase(0, request.find("Content-Length"));
    request.erase(request.find("\n"));
    content_length = string_to_int(request.substr(request.rfind(" ") + 1));
    return (content_length);
}