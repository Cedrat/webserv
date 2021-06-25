/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 15:43:38 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/25 18:37:45 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <unistd.h>
# include <cstring>
# include <cerrno>
# include <vector>

# include "Socket.hpp"
# include "PollSocket.hpp"
# include "fonction.hpp"

typedef int fd;

class Server
{
  public:
    Server( std::vector<int> ports, int host );
    Server( Server const & src );
    Server & operator=( Server const & rhs );
    ~Server();

    void  accept_connections();
    void  init();
    void  run();
    void  add_client( int new_fd );
    void receive_data( int i );

    bool compress_fds();

    //Getters
    fd*         getSockets() const;
    int         getSocketsNb() const;
    int         getHost() const;
    int         getStatus() const;
    pollfd      getPollFd( int i ) const;
    std::vector<int>  getPorts() const;

  private:
    Server();

    fd*     _socket;
    int     _host;
    int     _running;
    int     _nfds;
    std::vector<int>    _port;

    PollSocket        p1;
    struct  pollfd*   _fds;

};

#endif
