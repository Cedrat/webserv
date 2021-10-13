#ifndef DEFINE_HPP
# define DEFINE_HPP

# define FALSE 0
# define TRUE 1

# define SERVER 0
# define CLIENT 1

# define BACKLOG 10

# define TIMEOUT 60

# define POLL_TIME 1 * 1000 // one second.

# define OK 200
# define NO_CONTENT 204
# define MOVED_PERMANENTLY 301
# define BAD_REQUEST 400
# define NOT_FOUND 404
# define METHOD_NOT_ALLOWED 405
# define ENTITY_TOO_LARGE 413
# define NOT_SUPPORTED 505


typedef int fd;

#define MAX_REQUEST_SIZE 8000
#define BUFFER_SIZE 10000


# include <cstdio> //For size_t define;

#endif