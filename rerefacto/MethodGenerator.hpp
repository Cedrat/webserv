#include "../includes/utils.hpp"
#include "Erreur.hpp"

class AMethod;
class MethodGenerator
{
    public :
        AMethod* generate(std::string method, int fd, std::string path, std::string header) const
        {
            return (new Erreur(fd, path, header));
        }
};