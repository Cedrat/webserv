#ifndef CUSTOMEXCEPTION_HPP
# define CUSTOMEXCEPTION_HPP



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
        return ("Server error");
    }
};

class UnableToSendException : public CloseSocketException
{
    public : 
        virtual const char* what() const throw()
        {
            return ("send failed.. but the socket was passed by poll?");
        }
};

class UnableToReadException : public CloseSocketException
{
    virtual const char* what() const throw()
    {
        return ("read failed.. but the socket was passed by poll?");
    }
};

class ParserExit : public std::exception
{
    virtual const char* what() const throw()
    {
        return ("Parser Error");
    }
};

#endif