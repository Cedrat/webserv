#ifndef UTILS_HPP
# define UTILS_HPP

# include <unistd.h>
# include <stdbool.h>
# include <iostream>
# include <string>
// # include "../srcs/Config.hpp"
// # include "../srcs/ResponseHTTP.hpp"
// # include "../srcs/Location.hpp"
// # include "../srcs/Request.hpp"
# include <sstream>
# include <sys/types.h>
# include <sys/socket.h>
# include <ftw.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fstream>
# include <ctime>
# include <vector>
#include <arpa/inet.h>

class Socket;
class Config;
class Location;
class Request;
class ResponseHTTP;

typedef int fd;
#define BUFFER_SIZE 1000000
#define BACKLOG 10
#define FALSE 0
#define TRUE 1


void add_config(const char *path_conf);
std::string redir_path(std::string path, std::string path_redir, std::string part_to_replace);
bool is_folder(const char * path);
size_t get_file_size(std::string path);
bool check_if_file_exist(std::string path);
std::string construct_path(std::string path, Location location);
Location find_best_location(Request request, Config config);
size_t find_index_best_config(std::vector<Config> configs, std::string host_name, size_t port);
void process_data(Request &request, std::vector<Config> configs);
void response_header(Request request, fd fd_to_send);
void response_error_header(int num_error, fd fd_to_send);
void timeout(Socket & socket);
void response_post(int num_code,  Config config, fd fd_to_answer);
int match_regex(char *request, char * motif);
size_t count_words(std::string str);
std::string response_error_header(int error,  Config config, fd fd_to_answer);
void   response_good_file(std::string path, fd fd_to_answer, bool ai);
std::string get_string_error(int num_error);
size_t nb_of_char_in_str(char character, std::string str);
std::string int_to_string(int nb);
std::vector<std::string> split_string(std::string str, std::string splitter);
bool    check_valid_path(std::string path);
std::string factorised_path(std::string path);

void delete_and_give_response(std::string path, fd current_fd);
void delete_f(const char *path);

std::string create_ai_page(const char * path);
std::string create_path(Request request, Config config);

std::string    decoding_http_string(std::string str);

int string_to_int(std::string str);
void create_file(std::string path_name, std::string buffer);

bool check_if_method_is_authorized(Request &request, Config config);
#endif