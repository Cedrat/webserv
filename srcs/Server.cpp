/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 15:43:42 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/25 19:05:28 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( std::vector<int> ports, int host ) : _host(host), _running(true) ,
   _nfds(0), _port(ports)
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
  this->_port = rhs.getPorts();
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

    for (size_t i = 0; i < _port.size(); i++)
      s1.addSocketsListening(_port[i], _host);
    _socket = s1.listSockets();
    _nfds = getSocketsNb();
}

/**********************************
Fonctions membres - Boucle serveur
**********************************/
void Server::run()
{
  _p1.init(_socket, _nfds, POLLIN);
  //Loop waiting for incoming connects or for incoming data
  //on any of the connected sockets
  while (this->_running == true)
  {
    _fds = _p1.AvailableSockets();
    if (_fds == 0)
    {
      this->_running = false;
      break;
    }
    acceptConnections(); //Gerer cas d'erreur. Return toujours 1 actuellement
  }

  _p1.closeAllSockets();
}

void Server::acceptConnections()
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
        if (this->_nfds < NB_CLIENT_MAX)
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

        addClient(new_fd);
      }
    }
    else
    {
      if (this->_running == true)
        receiveData(i);
    }
  }
}

void Server::receiveData( int i )
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
    _p1.closeOneSocket(_fds[i]);
  }

}


/*******************************
Fonctions membres - Utilitaires
*******************************/

void Server::addClient( int new_fd )
{
  _p1.addClient(new_fd);
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

std::vector<int> Server::getPorts() const
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
