#include "SocketClient.hpp"

SocketClient::SocketClient(void) : _nb_sockets(0)
{

}


SocketClient::~SocketClient()
{

}

void SocketClient::addSocketClient(fd sockfd)
{
	_array_of_socket.push_back(sockfd);
	_nb_sockets++;
}

void SocketClient::closeAndRemoveSocket(size_t idx)
{
	close(_array_of_socket[idx]);
	
	std::vector<fd>::iterator it_start;
	it_start = _array_of_socket.begin();
	it_start += idx;
	_array_of_socket.erase(it_start);
	_nb_sockets--;
}

fd & SocketClient::operator[](size_t idx)
{
	return (_array_of_socket[idx]);
}

const fd& SocketClient::operator[](size_t idx) const
{
	return (_array_of_socket[idx]);
}