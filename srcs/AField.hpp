#ifndef AFIELD_HPP
#define AFIELD_HPP

#include "../includes/utils.hpp"
#include "RequestInProgress.hpp"

class AField
{
    protected : 
        std::string _method;
        std::string _path;
        std::string _host_name;

        std::string _final_path;

        std::string _str_request;
        
        std::string _header;
        
        RequestInProgress _data_request;

        int _error;

        void verifyRedirect(Location const &location);
        AMethod * createRedirMethod(Config config, Location location);
    
    public : 

        AField(std::string str_request, RequestInProgress data_request);
        virtual ~AField();
        std::string const & getMethod() const;
        std::string const & getPath() const;
        std::string const & getHostName() const;
        std::string const & getStrRequest() const;
        std::string const & getHeader() const;

        RequestInProgress const & getDataRequest() const;

        bool methodNotAuthorized(Location const & location);

        virtual void fillField() = 0;

        virtual AMethod * getAMethod() = 0;
};


class PostField : public AField
{
    private: 
        std::string _transfert_encoding;
        int         _content_length;
    
    public : 
        std::string const & getTransfertEncoding(){
            return (_transfert_encoding);
        }
        void setTransfertEncoding(std::string const transfert_encoding){
            _transfert_encoding = transfert_encoding;
        }
        int const & getContentLength(){
            return (_content_length);
        }
        void setContentLength(int const length){
            _content_length = length;
        }
};

class DeleteField : public AField
{

};

class RedirectField : public AField
{
    private :
        std::string _location; // Pas besoin? Si on prepare les headers a l'avance
};

class ErrorField : public AField
{

};

#endif