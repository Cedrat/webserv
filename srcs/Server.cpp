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

Server::Server( int port, int host ) : _port(port), _host(host), _socket(-1)
{
  if (init() == -1)
    exit(errno);
}

Server::Server( Server const & src )
{

}

Server & Server::operator=( Server const & rhs )
{

}

Server::~Server()
{

}

/*
Fonctions membres
*/
int Server::init()
{
    this->_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_socket < 0)
    {
      std::cerr << "Couldn't connect socket." << std::endl;
      return (-1);
    }

    setAddress();
    if (bind( this->_socket, (sockaddr *)&this->_address, sizeof(this->_address)) < 0)
    {
      std::cerr << "Couldn't bind port." << std::endl;
      return (-1);
    }

    if (listen(this->_socket, 1000) < 0)
    this->_listening = listen(this->_socket, SOMAXCONN);
    {
      std::cerr << "Couldn't listen." << std::endl;
      return (-1);
    }
}

void Server::setAddress()
{
  this->_sockAddr.sin_family = AF_INET;
  this->_sockAddr.sin_port = htons(this->_port);
  this->_sockAddr.sin_addr.s_addr = _host
}

int Server::accept()
{
  /*
  FD_ZERO(&this->_master); //Set to 0
  FD_SET(this->_socket, &this->_master); //Add first socket to master

  fd_set  copy = this->_master;
  int     socket_count = select(0, &copy, nullptr, nullptr, nullptr);

  //Loop through all current connexions
  for(int i = 0; i < socket_count; i++)
  {
    int sock = copy.fd_array[i];

    //Is it an inbound communication ?
    if(sock == this->_socket)
    {
      //Accept new comm
      int client = accept(this->_socket, nullptr, nullptr);
      FD_SET(client, &this->_master);

      //Envoyer message au client
      std::string hi = "Hello !";
      send(client, hi.c_str(), hi.size() + 1, 0);
    }
    else //Inbound message
    {
      char buf[4096];

      //Receive message
      int bytesIn = recv(sock, buf, 4096, 0);
      if (bytesIn <= 0)
      {
        //Drop the client
        close(sock);
        FD_CLR(sock, &this->_master);
        //TODO : Add message
      }
      else
      {
        //Check to see if it's a command
        if (buf[0] == '\\')
        {
          std::string cmd = std::string(buf, bytesIn);
          if (cmd == "\\quit")
          {
            break ;
          }
          continue;
        }
        //
      }
    }
  }

  //Remove listening socket from master file descriptor set and close it
  FD_CLR(this->_socket, &this->_master);
  close(this->_socket);

  while (&this->_master.fd_count > 0)
  {
    int sock = &this->_master.fd_array[0];
    std::string bye = "Bye !";

    send(sock, bye.c_str(), bye.size() + 1, 0);
    FD_CLR(sock, &this->_master);
    close(sock);
  }
  */
}
