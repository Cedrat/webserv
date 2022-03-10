#ifndef GENERATOR_HPP
# define GENERATOR_HPP

#include "../includes/utils.hpp"
#include "../methods/Erreur.hpp"
#include "../parsing/RequestInProgress.hpp"
#include "../fields/FieldGet.hpp"
#include "../fields/FieldDelete.hpp"
#include "../fields/FieldPost.hpp"

class AMethod;
class AField;
class FieldGenerator
{
    public :
        AField *generate(std::string method, std::string str_request, RequestInProgress const & data_request, pollfd &s_pollfd)
        {
            if (method == "GET")
                return (new FieldGet(str_request, data_request, s_pollfd));
            else if (method == "POST")
                return (new FieldPost(str_request, data_request, s_pollfd));
            else if (method == "DELETE")
                return (new FieldDelete(str_request, data_request, s_pollfd));
            return (new FieldGet(str_request, data_request, s_pollfd));
        }
};

#endif