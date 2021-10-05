#include "includes/utils.hpp"

int main()
{
    int fd = open("./new",  O_APPEND| O_CREAT | O_RDWR, 777);

    write(fd, "salut", 5);
    

    close(fd); 
}