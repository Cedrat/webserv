/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSocket.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 11:19:56 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/26 15:01:04 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLSOCKET_HPP
# define POLLSOCKET_HPP

# include "../includes/VectorUp.hpp"
# include "../includes/fonction.hpp"

# define TIMEOUT 3*60*1000


class PollSocket
{
  public:
    PollSocket();
    ~PollSocket();

    std::vector<struct pollfd> AvailableSockets();

    void init( fd* array_fd, int nfds, short events );

    void addClient( fd new_fd );
    void closeAllSockets();
    void closeOneSocket( pollfd toClose );


  private:
    VectorUp<pollfd>    _fds;
    int                 _nfds;
    short               _events;

};

#endif
