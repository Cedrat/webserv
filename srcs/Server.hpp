/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchampda <dchampda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 15:43:38 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/18 15:43:39 by dchampda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
#include <sys/ioctl.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/poll.h>
# include <unistd.h>
# include <cstring>
# include <cerrno>

class Server
{
  public:
    Server( int port, int host );
    Server( Server const & src );
    Server & operator=( Server const & rhs );
    ~Server();

    int   init();
    int   ft_poll();
    int   accept_connections();
    int   actions( int i );

    void  run();
    void setAddress();
    void close_fds();
    void add_client( int new_fd );

    bool compress_fds();
    bool receive_data( int i );

    //Getters
    int         getSocket() const;
    int         getPort() const;
    int         getHost() const;
    int         getStatus() const;
    sockaddr_in getSockAddr() const;
    pollfd      getPollFd( int i ) const;

  private:
    Server();

    int     _socket;
    int     _port;
    int     _host;
    int     _running;
    int     _nfds;
    struct  sockaddr_in _sockAddr;
    struct  pollfd      _master[1000];


};

#endif
