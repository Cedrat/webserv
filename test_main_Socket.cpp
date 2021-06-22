#include "Socket.hpp"
int main()
{
    Socket list_of_socket;

    list_of_socket.add_sockets_listening(7996);
    list_of_socket.add_sockets_listening(7995);
    list_of_socket.add_sockets_listening(7997);
    list_of_socket.add_sockets_listening(7998);
    
    fd *list_socket;
    list_socket = list_of_socket.list_sockets();
    for (int i = 0; list_socket[i] != ENDOFARRAY; i++)
    {
        std::cout << list_socket[i] << std::endl;
    }
}