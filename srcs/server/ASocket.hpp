#ifndef ASOCKET_HPP
# define ASOCKET_HPP

#include "../includes/utils.hpp"

class ASocket 
{
    protected :
        size_t  _port;
        int     _host;
        int     _timeout;

		int		_type;

    public :
		ASocket(size_t port, int host, int timeout,int type);
        int _fd;
        virtual ~ASocket();

        int const &     getTimeout() const;
        size_t const &  getPort() const;
        int const &     getHost() const;
		int const &		getType() const;

		void			setTimeout(int);

        virtual void exec() = 0;

};

#endif