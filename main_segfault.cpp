#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include "main_segfault.hpp"


pollfd create()
{
	struct pollfd lop;

	lop.fd = 1;
	lop.events = 0;
	lop.revents = 0;

	return(lop);
}

pollfd * convert(std::vector<pollfd> conv)
{
	return(conv.data());
}

int main(int argc)
{
    std::vector<pollfd> more_int;

	for (int i = 0; i < argc; i++)
    {
           more_int.push_back(create());
    }
	pollfd * test = convert(more_int);
    for (int i = 0; i < more_int.size(); i++)
    {
       std::cout << test[i].fd << std::endl;
    }

}
