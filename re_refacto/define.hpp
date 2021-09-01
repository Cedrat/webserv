#ifndef DEFINE_HPP
# define DEFINE_HPP

# define FALSE 0
# define TRUE 1

# define SERVER 0
# define CLIENT 1

# define BACKLOG 10

# define TIMEOUT 60

# define OK 200
# define BAD_REQUEST 400
# define NOT_FOUND 404
# define METHOD_NOT_ALLOWED 405
# define NOT_SUPPORTED 505

# define NO_METHOD -1
# define GET 0
# define DELETE 1
# define POST 2

typedef int fd;

# include <cstdio> //For size_t define;

#endif