NAME = webserv

PATH_SRC = ./srcs/
SRCS =  AField.cpp                      ConfigParser.cpp                Location.cpp                    SocketClient.cpp                check_if_file_exist.cpp         create_ai.cpp                   get_file_size.cpp  	             regexp.cpp \
AMethod.cpp                     Erreur.cpp                      MethodGet.cpp                   SocketServer.cpp                check_syntax_request.cpp        duplicata.cpp                   is_folder.cpp                    string_manip.cpp \
ASocket.cpp                     FieldGet.cpp                    RequestInProgress.cpp           Syntax.cpp                      check_timeout.cpp               error_tables.cpp                main.cpp					\
Config.cpp                      Formatting.cpp                  Server.cpp                      add_config.cpp                  construct_path.cpp              find_index_best_config.cpp      path_security.cpp		 MethodAi.cpp redir_path.cpp \
FieldDelete.cpp					delete_f.cpp 					FieldPost.cpp					date_string.cpp					is_not_number.cpp				trim_fields.cpp 				remove_chars_after_the_last_token.cpp
#PATH_CONFIG = ./config/
#CONFIG = ConfigParser.cpp        locationConfig.cpp      add_config.cpp         serverConfig.cpp

PATH_OBJS = ./objs/
OBJS = $(addprefix ${PATH_SRC},${SRCS:.cpp=.o})
OBJS_CONFIG = $(addprefix ,${CONFIG:.cpp=.o})
DEBUG = -g3  -fsanitize=address -fsanitize=undefined

CXX= clang++

CFLAGS = -Wall -Wextra  -std=c++98 -Weffc++

RM = rm -f

%.o :%.cpp
	$(CXX) $(CFLAGS) ${DEBUG} -c $< -o $@

all : 		${NAME}


${NAME}:	${OBJS} ${OBJS_CONFIG}
			$(CXX) ${CFLAGS} ${DEBUG} ${OBJS}  -o $@

clean :		
			${RM} ${OBJS}

fclean :	clean
			${RM} ${NAME}

re :		fclean all

.PHONY : all clean fclean re