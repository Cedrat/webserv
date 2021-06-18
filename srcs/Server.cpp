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
  bzero(&(this->_sockAddr.sin_zero), 8);
}

int Server::accept()
{
  //https://www.ibm.com/docs/en/i/7.1?topic=designs-using-poll-instead-select

  /*
  int nfds = 1;

  //Init pollfd structure
  memset(this->_master, 0, sizeof(this->_master));

  //Set up the initial listening socket
  this->_master[0].fd = listen_sd;
  this->_master[0].events = POLLIN;

  //initialize timeout to 3 mn
  int timeout = (3 * 60 * 1000);

  //Loop waiting for incoming connects or for incoming data
  //on any of the connected sockets
  do
  {
    //Call poll and wait 3mn for it to compelete
    int socket_count = poll(this->_master, 1000, timeout);

    //Check to see if any call failed
    if (socket_count < 0)
    {
      std::cerr << "Error in poll()" << std::endl;
      break;
    }

    //Check to see if the timeout expired
    if ( socket_count == 0)
    {
      std::cerr << "poll() timeout" << std::endl;
      break;
    }

    //One or more fd readables. Need to determine wich ones
    int current_size = socket_count;
    for (int i = 0; i < current_size; i++)
    {
      //Loop through to find the descriptors that returned POLLIN
      //and determine wether it's the listening or the active connection
      if (this->_master[i].revents == 0)
        continue;

      //If revents is not POLLIN it's an unexpected result, log and end server
      if (this->_master[i].revents != POLLIN)
      {
        std::cerr << "Error revents !" << std::endl;
        //end_server
        break ;
      }
      if (this->_master[i].fd == this->_socket)
      {
        //Listening descriptor is readable
        std::cout << "Listening descriptor is readable" << std::endl;

        //Accept all incoming conenctions that are queued up on the listening
        //socket before we loop back and call poll again
        do
        {
          //Accept each incoming conenction. If accept fails with EWOULDBLOCK
          //then we have accepted all of them. Any other failure on accept
          //will cause us to end the server
          int new_sd = accept(this->_socket, NULL, NULL);
          if (new_sd < 0)
          {
            if(errno != EWOULDBLOCK)
            {
              std::cerr << "accept() failed" << std::endl;
              //endserver = true;
            }
            break;
          }

          //Add the new incoming connection to the pollfd structure
          this->_master[nfds].fd = new_sd;
          this->_master[nfds].events = POLLIN;
          nfds++;

          //Loop back up and accept another incoming connection
        }
        while (new_sd != -1)
      }

      //This is not the listening socket, therefore an existing conenction
      //must be readable
      else
      {
        //close_conn = false;

        do
        {
          //Receive data on this connection until the recv fails with EWOULDBLOCK
          //If any other failure occurs we close the connection
          int rc = recv(this->_master[i].fd, buffer, sizeof(buffer), 0);
          if (rc < 0)
          {
            if (errno != EWOULDBLOCK)
            {
              std::cerr << "recv() failed" << std::endl;
              //close_conn = true;
            }
            break;
          }

          //Check to see if connection was closed by the client
          if (rc == 0)
          {
            std::cerr << "Conenction closed" << std::endl;
            //close_conn = true;
            break;
          }

          //data was received
          int len = rc;
          std::cout << "Bytes received" << std::endl;

          //Echo the data back to the client
          rc = send(this->_master[i].fd, buffer, len, 0);
          if (rc < 0)
          {
            std::cerr << "send() failed" << std::endl;
            //close_conn = true;
            break;
          }
        }
        while(TRUE);

        //If the close_conn flag was turned on we need to clean up
        //this active conenction. This clean up process includes removing the fd
        if (close_conn)
        {
          close(this->_master[i].fd);
          this->_master[i].fd = -1;
          //compress_array = true;
        }
      } //End of existing connection is readable
    } //end of loop throuogh pollable descriptors

    //If the compress_array flag was turned on, we need to squeeze together
    //the array and decrement the number of fd. We do not need to move back
    //the events and revents fields because the event will always
    //be POLLIN in this case and revents is output
    if (compress_array)
    {
      compress_array =false;
      for (int i = 0; i < nfds; i++)
      {
        if (this->_master[i].fd == -1)
        {
          for (j = i; j < nfds; j++)
            this->_master[j].fd = this->_master[j + 1].fd;
          i--;
          nfds--;
        }
      }
    }
  } while (end_server == false); //end of server running

  //Clean up all of the sockets that are open
  for (int i = 0; i < nfds; i++)
  {
    if (this->_master[i].fd >= 0)
      close(this->_master[i].fd);
  }

  */
}
