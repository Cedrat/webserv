#ifndef VERIFYDUPLICATA_HPP
# define VERIFYDUPLICATA_HPP

#include "../includes/utils.hpp"

class VerifyDuplicata
{
    private :  
        int _nb_host;
        int _nb_content_length;

    public : 
        VerifyDuplicata(std::string request);
        ~VerifyDuplicata();

        int const & getNbHost() const;
        int const & getNbContentLength() const;

        bool const isDuplicata() const;
        bool const isMissingData() const;
};

#endif