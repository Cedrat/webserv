#ifndef GENERATOR_HPP
# define GENERATOR_HPP

#include "../includes/utils.hpp"
#include "Erreur.hpp"
#include "RequestInProgress.hpp"
#include "FieldGet.hpp"
#include "FieldDelete.hpp"

class AMethod;
class AField;
class MethodGenerator
{
    public :
        AMethod* generate(std::string method, int fd, std::string path, std::string header) const
        {
            return (new Erreur(fd, path, header));
        }
};

class FieldGenerator
{
    public :
        AField *generate(std::string method, std::string str_request, RequestInProgress const & data_request)
        {
            if (method == "GET")
                return (new FieldGet(str_request, data_request));
            else if (method == "DELETE")
                return (new FieldDelete(str_request, data_request));
            throw ("bad method");
        }
};

#endif