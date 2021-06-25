/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollSocket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/25 11:19:53 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/25 18:39:32 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PollSocket.hpp"

PollSocket::PollSocket() {}

PollSocket::~PollSocket() {}

void PollSocket::init( fd* fd_array, int nfds, short events )
{
  struct pollfd new_fd;

  this->_nfds = nfds;
  this->_events = events;

  for (int i = 0; i < this->_nfds; i++)
  {
    new_fd.fd = fd_array[i]; //TO_DO : error_managembbent
    new_fd.events = this->_events;
    _fds.push_back(new_fd);
  }
}


struct pollfd* PollSocket::AvailableSockets()
{
  int socket_count;
  struct pollfd* fds = _fds.data();

  //Call poll and wait 3mn for it to complete
  std::cout << "Waiting for poll()" << std::endl;
  socket_count = poll(fds, this->_nfds, TIMEOUT);

  if (socket_count < 0) //Check to see if any call failed
  {
    std::cerr << "Error in poll()" << std::endl;
    return (NULL);
  }
  if (socket_count == 0)  //Check to see if the timeout expired
  {
    std::cerr << "poll() timeout" << std::endl;
    return (NULL);
  }
  return (fds);
}


void PollSocket::addClient( fd new_fd )
{
  pollfd new_client;

  std::cout << "New connection, socket fd is " << new_fd << "\n" << std::endl;
  new_client.fd = new_fd;
  new_client.events = _events;
  /* Pour Linux */
  fcntl(new_client.fd, F_SETFL, O_NONBLOCK);
  /* FY Linux  */
  this->_nfds++;

  _fds.push_back(new_client);
}



void PollSocket::closeAllSockets()
{
  for (int i = 0; i < _nfds; i++)
  {
    if (_fds[i].fd >= 0)
      close(_fds[i].fd);
  }
}

void PollSocket::closeOneSocket( struct pollfd toClose )
{
  close(toClose.fd);
  toClose.fd = -1;
}
