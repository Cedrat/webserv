#ifndef UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <stdbool.h>
# include <iostream>
# include <string>
# include "../srcs/Config.hpp"
# include "../srcs/Location.hpp"

class Config;
class Location;

typedef int fd;
#define BUFFER_SIZE 128
#define BACKLOG 10
#define FALSE 0
#define TRUE 1

int match_regex(char *request, char * motif);
size_t count_words(std::string str);
void response_error_header(int error,  Config config, fd fd_to_answer);
void response_good_file(std::string path, fd fd_to_answer);
std::string get_string_error(int num_error);
size_t nb_of_char_in_str(char character, std::string str);

#endif