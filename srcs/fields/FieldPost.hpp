#ifndef FIELDPOST_HPP
# define FIELDPOST_HPP

#include "../includes/utils.hpp"
#include "AField.hpp"
#include "../methods/MethodPostCgi.hpp"

class FieldPost : public AField
{
    private: 
        std::string _transfert_encoding;
        std::string _str_content_length;
        int         _content_length;
        std::string _content_type;

        AMethod *createErrorMethod(Config config, Location location);
        AMethod *createCgiMethod(Config config, Location location);
        std::string createPathUploadFolder(std::string upload_folder);
        void    checkValidPath(Location location);
        void    checkBodySize(Config const & config);
        void    verifyPath(); 
    
public :
        FieldPost(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd);
        ~FieldPost();

        void setContentLength(std::string const & length);
        void setTransfertEncoding(std::string const transfert_encoding);
  
        std::string const & getStrContentLength() const;
        std::string const & getContentType() const;
        int const & getContentLength() const ;
        std::string const & getTransfertEncoding() const;

        void fillField();
        AMethod *getAMethod();

        void verifyMissingData();
        void verifyData();

};

#endif