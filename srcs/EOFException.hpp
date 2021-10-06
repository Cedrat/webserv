#ifndef EOFEXCEPTION_HPP
# define EOFEXCEPTION_HPP

class EOFException : public std::exception
{
    virtual const char* what() const throw()
    {
        return ("EOF");
    }
};

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

#endif