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
        std::vector<Config> _configs;

        int         checkBasicError();
        int         checkCommonError();

    public :
        RequestInProgress();
        ~RequestInProgress();

        std::string const & getRequest() const;
        AMethod *           getAnswer();

        void                setFd(int fd);

        void        checkFinished(); 
        
        bool const isFinished();
        
        void receiveData();
        
        void addToRequest(std::string str_request);
        
        void        reset();

        void setConfigs(std::vector<Config> configs);
};

#endif