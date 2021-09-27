#include "Erreur.hpp"
#include "define.hpp"
#include "AField.hpp"

Erreur::Erreur(int fd, std::string path, std::string header, AField &field) : 
AMethod(fd, path, header, field)
{

}

Erreur::~Erreur()
{

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
        std::cout << "PATH : " << getPath() << std::endl; 
        fs.open(getPath().c_str(),  std::fstream::in | std::fstream::app); 
        //fs.seekg(_byte_send);
        fs.seekg(0);
        fs.read(buffer, BUFFER_SIZE);
        buffer[fs.gcount()] = '\0'; 
        std::cout << "how much read? " << fs.gcount() << std::endl;
        ret = ::send(getFd(), buffer, fs.gcount(), 0);
        //_byte_send += ret;
        // if (ret == fs.gcount() && fs.eof())
        // {
        //     _finished = TRUE;
        // }
         std::cout << ret << "BYTE SEND" << std::endl;
        fs.close();
}