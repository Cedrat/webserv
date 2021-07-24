#ifndef UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <stdbool.h>
# include <iostream>
# include <string>
# include "../srcs/Config.hpp"

class Config;

typedef int fd;
#define BUFFER_SIZE 128
#define BACKLOG 10
#define FALSE 0
#define TRUE 1

int match_regex(char *request, char * motif);
size_t count_words(std::string str);
void response_error_header(int error,  Config config, fd fd_to_answer);

#endif