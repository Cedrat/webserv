/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 15:43:38 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/25 19:02:22 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../includes/fonction.hpp"
# include "Socket.hpp"
# include "PollSocket.hpp"

# define NB_CLIENT_MAX 1000

class Server
{
  public:
    Server( std::vector<int> ports, int host );
    Server( Server const & src );
    Server & operator=( Server const & rhs );
    ~Server();

    void  acceptConnections();
    void  init();
    void  run();
    void  addClient( int new_fd );
    void  receiveData( int i );

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

    PollSocket        _p1;
    struct  pollfd*   _fds;

};

#endif
