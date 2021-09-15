#include "define.hpp"
#include "SocketClient.hpp"

SocketClient::SocketClient(size_t port, int host) 
: ASocket(port, host, TIMEOUT, CLIENT)
{

}