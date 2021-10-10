#include "Erreur.hpp"
#include "define.hpp"
#include "AField.hpp"
#include "EOFException.hpp"

Erreur::Erreur(int fd, std::string path, std::string header, AField &field, int error) : 
AMethod(fd, path, header, field) , _error(error)
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

void Erreur::sendHeader()
{
   send(getFd(), getHeader().c_str(), getHeader().size(), 0); 
}

void Erreur::sendBody()
{
    std::fstream fs;
    char buffer[BUFFER_SIZE + 1];
    int ret = 0;
    
    fs.open(getPath().c_str(),  std::fstream::in | std::fstream::app); 
    fs.seekg(0);
    fs.read(buffer, BUFFER_SIZE);
    buffer[fs.gcount()] = '\0'; 
    ret = ::send(getFd(), buffer, fs.gcount(), 0);
    if (ret == fs.gcount() && fs.eof())
    {
        _is_finished = TRUE;
        if (_error == BAD_REQUEST)
        {
            fs.close();
            throw(BadRequestException());
        }
    }
    fs.close();
}