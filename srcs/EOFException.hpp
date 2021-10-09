#ifndef EOFEXCEPTION_HPP
# define EOFEXCEPTION_HPP



class EmergencyExit : public std::exception
{
    virtual const char* what() const throw()
    {
        return ("EOF");
    }
    void quitProgram() const throw()
    {
        return ;
    }
};

class CloseSocketException : public std::exception
{

};

class BadRequestException : public CloseSocketException
{
    virtual const char* what() const throw()
    {
        return ("Bad Request Exception");
    }
};

class FileDisappearedException : public CloseSocketException
{
    virtual const char* what() const throw()
    {
        return ("file disappeared");
    }
};

class EOFException : public CloseSocketException
{
    virtual const char* what() const throw()
    {
        return ("EOF");
    }
};

class UnableToSendException : public CloseSocketException
{
    virtual const char* what() const throw()
    {
        return ("send failed.. but the socket was passed by poll?");
    }
};

#endif