#ifndef ERREUR_HPP
# define ERREUR_HPP

#include "AMethod.hpp"

class Erreur : public AMethod
{
    private:
        void sendHeader();
        void sendBody();
        int _error;

    public :
        Erreur(int fd, std::string path, std::string header, AField &fields, int error);
        ~Erreur();
        void init();
        void exec();
};

#endif