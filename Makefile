NAME = webserv

PATH_SRC = ./srcs/
SRCS =  Config.cpp                              Server.cpp                              check_if_method_is_authorized.cpp       error_tables.cpp                        main.cpp                                regexp.cpp \
Location.cpp                            Socket.cpp                              construct_path.cpp                      find_index_best_config.cpp              path_security.cpp                       response_error_header.cpp \
Request.cpp                             VerifyDuplicata.cpp                     create_ai.cpp                           get_file_size.cpp                       process_data.cpp                        string_manip.cpp \
ResponseHTTP.cpp                        check_if_file_exist.cpp                 delete_f.cpp                            is_folder.cpp                           redir_path.cpp                          timeout.cpp \
Syntax.cpp		Formatting.cpp			set_responseHTTP_error.cpp

PATH_CONFIG = ./config/
CONFIG = ConfigParser.cpp        locationConfig.cpp      add_config.cpp         serverConfig.cpp

PATH_OBJS = ./objs/
OBJS = $(addprefix ${PATH_SRC},${SRCS:.cpp=.o})
OBJS_CONFIG = $(addprefix ${PATH_CONFIG},${CONFIG:.cpp=.o})
DEBUG = -g3 -fsanitize=address

CXX= clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

%.o :%.cpp
	$(CXX) $(CFLAGS) ${DEBUG} -c $< -o $@

all : 		${NAME}


${NAME}:	${OBJS} ${OBJS_CONFIG}
			$(CXX) ${CFLAGS} ${DEBUG} ${OBJS} ${OBJS_CONFIG} -o $@

clean :		
			${RM} ${OBJS}

fclean :	clean
			${RM} ${NAME}

re :		fclean all

.PHONY : all clean fclean re
