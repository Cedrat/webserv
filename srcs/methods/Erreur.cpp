#include "Erreur.hpp"
#include "../includes/define.hpp"
#include "../fields/AField.hpp"
#include "../includes/CustomException.hpp"

Erreur::Erreur(int fd, std::string path, std::string header, AField &field) : 
AMethod(fd, path, header, field)
{

}

Erreur::~Erreur()
{
    delete &_fields;
}
void Erreur::init() 
{
    _fields.setPollout();
}

void Erreur::exec()
{
    if (getHeaderSent() == FALSE)
    {
        sendHeader();
        setHeaderSent(TRUE);
    }
    else
    {
        sendBody();
        setIsFinished(TRUE);
    }
}
