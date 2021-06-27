/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lnoaille <lnoaille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/18 15:43:42 by dchampda          #+#    #+#             */
/*   Updated: 2021/06/27 17:42:54 by lnoaille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server( std::vector<int> ports, int host ) : _port(ports), _host(host),
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
    FdInformation socket_server;

    socket_server.setTypeOfSocket(SOCKET_SERVER);
    for (size_t i = 0; i < _port.size(); i++)
    {
      s1.addSocketsListening(_port[i], _host);
      _fd_info.push_back(socket_server);
    }
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
    try
    {
      _fds = _p1.AvailableSockets();
      acceptConnections();
    }
    catch (char const * &e) 
    {
      std::cout << e << std::endl;
      this->_running = false;
    }
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
    if (_fd_info[i].getTypeOfSocket() == SOCKET_SERVER)
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
          std::cerr << strerror(errno) << std::endl;
          std::cerr << "accept() failed" << std::endl;
          this->_running = false;
          break;
        }
        if (send(new_fd, "Hello !\n", strlen("Hello !\n"), 0) != strlen("Hello !\n"))
          std::cerr << "Error sending greeting message" << std::endl;
        std::cout << "Greeting message sent successfully !" << std::endl;
        
        addClient(new_fd);
        break;
    }
    else if (this->_fds[i].fd > 0)
    {
      if (this->_running == true)
        receiveData(i);
    }
  }
}
static bool is_close(int bytes_read)
{
  if (bytes_read == 0) //POLLHUP aussi mais doute ? Empeche certaines choses.
  {
    std::cerr << "Connection closed" << std::endl;
    return (true);
  }
  return (false);
}

static void recv_error_management(int bytes_read)
{
  if (bytes_read < 0)
  {
    std::cerr << "recv() failed" << std::endl;
  }
}
void Server::receiveData( int i )
{
  int bytes_read;
  char  buffer[1000];
  bool  close_conn = false;


    //Receive data until the client close his connection or an error occurs
    std::cout << "Receive data from " << this->_fds[i].fd << std::endl; 
      bytes_read = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
    
    close_conn = is_close(bytes_read);
    std::cout << bytes_read << " bytes received\n" << std::endl;
    if (bytes_read < 0)
    {
      if (_fds[i].revents & POLLERR)
      {
        std::cerr << "recv() failed " << std::endl;
        close_conn = true;
      }
    }
    //Data was received
   //
      int len = bytes_read;
      //Echo the data back to the client
      bytes_read = send(this->_fds[i].fd, buffer, len, 0);
      if (bytes_read < 0 && _fds[i].revents & POLLHUP)
      {
        std::cerr << "send() failed" << std::endl;
        close_conn = true;
      }

  if (close_conn == true)
  {
    _p1.closeOneSocket(_fds[i]);
    _fd_info.pop_back();
  }

}

/*******************************
Fonctions membres - Utilitaires
*******************************/

void Server::addClient( int new_fd )
{
  FdInformation socket_client; 

  socket_client.setTypeOfSocket(SOCKET_CLIENT);
  _p1.addClient(new_fd);
  _fd_info.push_back(socket_client);
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
