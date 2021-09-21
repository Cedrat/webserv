#ifndef REQUESTINPROGRESS_HPP
# define REQUESTINPROGRESS_HPP

# include "../includes/utils.hpp"
# include "AMethod.hpp"
# include "Config.hpp"

class RequestInProgress
{
    private :
        std::string         _str_request;
        bool                _is_finished;
        
        int                 _socket_fd;
        int                 _host;
        size_t              _port;
        std::vector<Config> _configs;

        int         checkBasicError();
        int         checkCommonError();

    public :
        RequestInProgress();
        ~RequestInProgress();

        std::string const &         getRequest() const;
        AMethod *                   getAnswer();
        std::vector<Config> const & getConfigs() const;
        size_t const &              getPort() const;
        int const &                 getHost() const;
        int const &                 getFd() const;

        void                setFd(int fd);
        void                setHost(int host);
        void                setPort(size_t port);

        void        checkFinished(); 
        
        bool isFinished();
        
        void receiveData();
        
        void addToRequest(std::string str_request);
        
        void        reset();

        void setConfigs(std::vector<Config> configs);
};

#endif