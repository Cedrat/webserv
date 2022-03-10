#ifndef DEFINE_HPP
# define DEFINE_HPP

# define FALSE 0
# define TRUE 1

# define SERVER 0
# define CLIENT 1

# define BACKLOG 100

# define TIMEOUT 60

# define POLL_TIME 1 * 1000 // one second.

# define OK 200
# define CREATED 201
# define NO_CONTENT 204
# define MOVED_PERMANENTLY 301
# define BAD_REQUEST 400
# define FORBIDDEN 403
# define NOT_FOUND 404
# define METHOD_NOT_ALLOWED 405
# define ENTITY_TOO_LARGE 413
# define SERVER_ERROR 500
# define NOT_SUPPORTED 505


typedef int fd;

#define MAX_REQUEST_SIZE 8000
#define BUFFER_SIZE 128000

#define PATH_TMP "./tmp/"


# include <cstdio> //For size_t define;

#endif