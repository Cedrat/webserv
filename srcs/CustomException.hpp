#ifndef CUSTOMEXCEPTION_HPP
# define CUSTOMEXCEPTION_HPP

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
        return ("Server error");
    }
    void quitProgram() const throw()
    {
        return ;
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