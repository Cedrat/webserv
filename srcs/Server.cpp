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
  _running(true), _nfds(1)
{
  if (init() == -1)
    return ;
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
  this->_sockAddr = rhs.getSockAddr();
  for (int i = 0; i < 1000; i++)
    this->_master[i] = rhs.getPollFd(i);
  return (*this);
}

Server::~Server() {}

/*
Fonctions membres
*/
int Server::init()
{
    /*
    Ajout des hosts
    */
    Socket s1;
    s1.add_sockets_listening(7994);
    s1.add_sockets_listening(7995);
    _socket = s1.list_sockets();
    return (1);
}

void Server::run()
{
  init_fds();

  //Loop waiting for incoming connects or for incoming data
  //on any of the connected sockets
  while (this->_running == true)
  {
    if (!ft_poll())
      this->_running = false;
    if (!accept_connections())
      this->_running = false;
  }
  close_fds();
}

void  Server::init_fds()
{
  //Init pollfd structure & set up the initial listening socket
  memset(this->_master, 0, sizeof(this->_master));
  for (size_t i = 0; _socket[i] != ENDOFARRAY; i++)
  {
    _master[i].fd = _socket[i];
    _master[i].events = POLLIN;
  }
  //this->_master[0].fd = this->_socket[0];
  //this->_master[0].events = POLLIN;
}

int Server::ft_poll()
{
  int timeout = (3 * 60 * 1000); // = 3 mn
  int socket_count;

  //Call poll and wait 3mn for it to complete
  std::cout << "Waiting for poll()" << std::endl;
  socket_count = poll(this->_master, this->_nfds, timeout);

  if (socket_count < 0) //Check to see if any call failed
  {
    std::cerr << "Error in poll()" << std::endl;
    return (-1);
  }
  if (socket_count == 0)  //Check to see if the timeout expired
  {
    std::cerr << "poll() timeout" << std::endl;
    return (-1);
  }
  return (1);
}

int Server::accept_connections()
{
  char buffer[80];
  int current_size = _nfds;
  int new_fd = 0;

  for (int i = 0; i < current_size; i++)
  {
    if (this->_master[i].revents == 0)
      continue;
    if (_master[i].fd == this->_socket[i])
    {
      while(this->_nfds != -1)
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
        std::cout << "Greeting message sent successfully !\n" << std::endl;
        add_client(new_fd);
      }
    }
    else
      receive_data(i);
  }
  return 1;
}

bool Server::receive_data( int i )
{
  int rc;
  char  buffer[80];
  bool  close_conn = false;
 
//TODO : Correction pb lors de fermeture du client

  while (1)
  {
    //Receive data on this connection until the recv fails with EWOULDBLOCK
    //If any other failure occurs we close the connection
    rc = recv(this->_master[i].fd, buffer, sizeof(buffer), 0);
    if (rc < 0)
    {
      if (errno != EWOULDBLOCK)
      {
        std::cerr << "recv() failed " << std::endl;
        close_conn = true;
      }
      break;
    }
    if (rc == 0) //Check to see if connection was closed by the client
    {
      std::cerr << "Connection closed" << std::endl;
      close_conn = true;
      break;
    }
    //Data was received
    int len = rc;
    std::cout << rc << " bytes received" << std::endl;

    //Echo the data back to the client
    rc = send(this->_master[i].fd, buffer, len, 0);
    if (rc < 0)
    {
      std::cerr << "send() failed" << std::endl;
      close_conn = true;
      break;
    }
  }
  if (close_conn == true)
  {
    close(_master[i].fd);
    _master[i].fd = -1;
    compress_fds();
  }
  return close_conn;
}

bool Server::compress_fds()
{
  for (int i = 0; i < _nfds; i++)
  {
    if (this->_master[i].fd == -1)
    {
      for (int j = i; j < _nfds; j++)
      {
        this->_master[j].fd = this->_master[j + 1].fd;
        this->_master[j].events = this->_master[j + 1].events;
      }
      i--;
      _nfds--;
    }
  }
  return false;
}

void Server::close_fds()
{
  //Clean up all of the sockets that are open
  for (int i = 0; i < _nfds; i++)
  {
    if (this->_master[i].fd >= 0)
      close(this->_master[i].fd);
  }
}

void Server::add_client( int new_fd )
{
  std::cout << "New connection, socket fd is " << new_fd << std::endl;

  this->_master[this->_nfds].fd = new_fd;
  this->_master[this->_nfds].events = POLLIN;
  this->_nfds++;
}


/*
Getters
*/
fd* Server::getSockets() const
{
  return (this->_socket);
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

sockaddr_in Server::getSockAddr() const
{
  return (this->_sockAddr);
}

pollfd  Server::getPollFd( int i ) const
{
  return (this->_master[i]);
}

/*


*/
