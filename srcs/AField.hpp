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
        std::string _query;

        std::string _str_request;
        
        std::string _header;
        
        RequestInProgress _data_request;

        pollfd & _pollfd;

        int _error;

        void verifyRedirect(Location const &location);
        AMethod * createRedirMethod(Config config, Location location);
    
    public : 

        AField(std::string str_request, RequestInProgress data_request, pollfd & s_pollfd);
        virtual ~AField();
        std::string const & getMethod() const;
        std::string const & getPath() const;
        std::string const & getHostName() const;
        std::string const & getStrRequest() const;
        std::string const & getHeader() const;
        std::string const & getQuery() const;
        virtual std::string const & getTransfertEncoding() const;
        virtual std::string const & getStrContentLength() const;
        virtual int const & getContentLength() const;

        RequestInProgress const & getDataRequest() const;

        void setPollout();
        void setPollin();

        bool methodNotAuthorized(Location const & location);

        virtual void fillField() = 0;

        virtual AMethod * getAMethod() = 0;
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