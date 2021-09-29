#ifndef EOFEXCEPTION_HPP
# define EOFEXCEPTION_HPP

class EOFException : public std::exception
{
    virtual const char* what() const throw()
    {
        return ("EOF");
    }
};

#endif