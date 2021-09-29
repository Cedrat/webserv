#ifndef FIELDPOST_HPP
# define FIELDPOST_HPP

#include "../includes/utils.hpp"
#include "AField.hpp"

class FieldPost : public AField
{
    private: 
        std::string _transfert_encoding;
        std::string _content_length;

        AMethod *createErrorMethod(Config config);
        std::string createPathUploadFolder(std::string upload_folder);
        void    checkValidPath();
        void    checkBodySize(Config const & config);    
    
public :
        FieldPost(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd);
        ~FieldPost();

        void setContentLength(std::string const & length);
        void setTransfertEncoding(std::string const transfert_encoding);
  
        std::string const & getContentLength();
        std::string const & getTransfertEncoding() const;

        void fillField();
        AMethod *getAMethod();

        void verifyMissingData();
        void verifyData();

};

#endif