/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dchampda <dchampda@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 15:43:42 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/18 15:43:43 by dchampda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( int port, int host ) : _port(port), _host(host),
  _running(true), _nfds(0)
{
  init();
  run();
}

Server::Server( Server const & src )
{
  if (this != &src)
    *this = src;
}

Server & Server::operator=( Server const & rhs )
{
  this->_socket = rhs.getSockets();
  this->_port = rhs.getPort();
  this->_host = rhs.getHost();
  this->_running = rhs.getStatus();
  for (int i = 0; i < 1000; i++)
    this->_fds[i] = rhs.getPollFd(i);
  return (*this);
}

Server::~Server() {}



/**********************************
Fonctions membres - Initialisation
***********************************/
void Server::init()
{
    //Ajout des hosts & init du set de fds
    Socket s1;

    s1.add_sockets_listening(7994);
    s1.add_sockets_listening(7995);
    _socket = s1.list_sockets();
    _nfds = getSocketsNb();
}

/**********************************
Fonctions membres - Boucle serveur
**********************************/
void Server::run()
{
  p1.init(_socket, _nfds, POLLIN);
  //Loop waiting for incoming connects or for incoming data
  //on any of the connected sockets
  while (this->_running == true)
  {
    _fds = p1.AvailableSockets();
    if (!_fds)
    {
      this->_running = false;
      break;
    }
    accept_connections(); //Gerer cas d'erreur. Return toujours 1 actuellement
  }

  p1.closeAllSockets();
}

void Server::accept_connections()
{
  int current_size = _nfds;
  int new_fd = 0;

  for (int i = 0; i < current_size; i++)
  {
    if (this->_fds[i].revents == 0)
      continue;
    if (_fds[i].fd == this->_socket[i])
    {
      while(new_fd != -1)
      {
        if (this->_nfds < 1000)
          new_fd = accept(this->_socket[i], NULL, NULL);
        else
        {
          std::cerr << "Max number of clients reached" << std::endl;
          break;
        }
        if (new_fd < 0)
        {
          if (errno != EWOULDBLOCK)
          {
            std::cerr << "accept() failed" << std::endl;
            this->_running = false;
          }
          break;
        }

        if (send(new_fd, "Hello !\n", strlen("Hello !\n"), 0) != strlen("Hello !\n"))
          std::cerr << "Error sending greeting message" << std::endl;
        std::cout << "Greeting message sent successfully !" << std::endl;

        add_client(new_fd);
      }
    }
    else
    {
      if (this->_running == true)
        receive_data(i);
    }
  }
}

void Server::receive_data( int i )
{
  int rc;
  char  buffer[80];
  bool  close_conn = false;

//TODO : Correction pb lors de fermeture du client

  while (1)
  {
    //Receive data until the client close his connection or an error occurs
    rc = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
    if (rc == 0 || _fds[i].revents & POLLHUP) //Check to see if connection was closed by the client
    {
      std::cerr << "Connection closed" << std::endl;
      close_conn = true;
      break;
    }
    if (rc < 0)
    {
      if (_fds[i].revents & POLLERR)
      {
        std::cerr << "recv() failed " << std::endl;
        close_conn = true;
      }
      break;
    }

    //Data was received
    int len = rc;
    std::cout << rc << " bytes received\n" << std::endl;
    //Echo the data back to the client
    rc = send(this->_fds[i].fd, buffer, len, 0);
    if (rc < 0)
    {
      std::cerr << "send() failed" << std::endl;
      close_conn = true;
      break;
    }
  }

  if (close_conn == true)
  {
    p1.closeOneSocket(_fds[i]);
    //compress_fds();
  }

}



/*******************************
Fonctions membres - Utilitaires
*******************************/
bool Server::compress_fds()
{
  for (int i = 0; i < _nfds; i++)
  {
    if (this->_fds[i].fd == -1)
    {
      for (int j = i; j < _nfds; j++)
      {
        this->_fds[j].fd = this->_fds[j + 1].fd;
        this->_fds[j].events = this->_fds[j + 1].events;
      }
      i--;
      _nfds--;
    }
  }
  return false;
}

void Server::add_client( int new_fd )
{
  p1.addClient(new_fd);
  this->_nfds++;
}


/***************************
Fonctions membres - Getters
***************************/
fd* Server::getSockets() const
{
  return (this->_socket);
}

int Server::getSocketsNb() const
{
  int i = 0;

  while (_socket[i] != ENDOFARRAY)
    i++;
  return (i);
}

int Server::getPort() const
{
  return (this->_port);
}

int Server::getHost() const
{
  return (this->_host);
}

int Server::getStatus() const
{
  return (this->_running);
}

pollfd  Server::getPollFd( int i ) const
{
  return (this->_fds[i]);
}
