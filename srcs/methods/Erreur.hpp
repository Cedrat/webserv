#ifndef ERREUR_HPP
# define ERREUR_HPP

#include "AMethod.hpp"

class Erreur : public AMethod
{
    private:

    public :
        Erreur(int fd, std::string path, std::string header, AField &fields);
        ~Erreur();
        void init();
        void exec();
};

#endif