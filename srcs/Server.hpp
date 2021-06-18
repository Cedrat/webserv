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
# include <sys/socket.h>

class Server
{
  public:
    Server();
    Server( Server const & src );
    Server & operator=( Server const & rhs );
    ~Server();

    int init();
    int accept();

    void setAddress();

  private:
    int     _socket;
    int     _port;
    int     _host;

    //fd_set              _master;
    struct  sockaddr_in _address;

};

#endif
