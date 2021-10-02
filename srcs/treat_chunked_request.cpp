#include "../includes/utils.hpp"

std::string process_chunked_request(std::string request)
{
    
    return "";
}


int main()
{
    std::string chunked_request("\r\n26\r\n"
                                "Voici les données du premier morceau\r\n\r\n"
                                "1C\r\n"
                                "et voici un second morceau\r\n\r\n"
                                "20\r\n"
                                "et voici deux derniers morceaux \r\n"
                                "12\r\n"
                                "sans saut de ligne\r\n"
                                "0\r\n"
                                "\r\n");

    std::cout << chunked_request << std::endl;
    std::string request_processed = process_chunked_request(chunked_request);
}