#include "../includes/utils.hpp"
#include <cmath>

static int pos_in_hexa_array(char value)
{
    char hexa_array[] = "0123456789ABCDEF";

    for (int i = 0; i < 17; i++)
    {
        if (hexa_array[i] == value)
            return (i);
    }
    throw "bad hexa value";
}
int atoi_hexa(std::string nb)
{
    size_t decimal_value = 0;

    for (size_t i = 0; i < nb.size(); i++)
    {
        decimal_value += pos_in_hexa_array(nb[i]) * std::pow(16, nb.size() - (i + 1));
    }    return decimal_value;
}


std::string process_chunked_request(std::string request)
{
    std::string str_nb_bytes_next;
    std::string processed_chunk;
    int         nb_bytes_next;

    while (request.find("\r\n") == 0)
    {
        request.erase(0, 2);
        str_nb_bytes_next = request.substr(0,request.find("\r\n"));
        request.erase(0, request.find("\r\n") + 2);
        
        nb_bytes_next = atoi_hexa(str_nb_bytes_next);
        processed_chunk += request.substr(0, nb_bytes_next);
        request.erase(0, nb_bytes_next);
    }
    return processed_chunk;
}


int main()
{
    std::string chunked_request("\r\n26\r\nVoici les donnees du premier morceau\r\n\r\n"
                                "1C\r\n"
                                "et voici un second morceau\r\n\r\n"
                                "20\r\n"
                                "et voici deux derniers morceaux \r\n"
                                "12\r\n"
                                "sans saut de ligne\r\n"
                                "0\r\n"
                                "\r\n");
    std::string request_processed = process_chunked_request(chunked_request);
    std::cout << request_processed << std::endl;
}