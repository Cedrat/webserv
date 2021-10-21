NAME = webserv

VPATH =objs:./srcs:srcs/methods:srcs/fields:srcs/server:srcs/config:srcs/includes:srcs/parsing:srcs/server:srcs/utils
#vpath %.hpp :./srcs:srcs/methods:./includes

SRCS =  AField.cpp                      ConfigParser.cpp                Location.cpp                    SocketClient.cpp                check_if_file_exist.cpp         create_ai.cpp                   get_file_size.cpp  	             regexp.cpp \
AMethod.cpp                     Erreur.cpp                      MethodGet.cpp                   SocketServer.cpp                check_syntax_request.cpp        duplicata.cpp                   is_folder.cpp                    string_manip.cpp \
ASocket.cpp                     FieldGet.cpp                    RequestInProgress.cpp           Syntax.cpp                      check_timeout.cpp               error_tables.cpp                main.cpp					\
Config.cpp                      Formatting.cpp                  Server.cpp                      construct_path.cpp              find_index_best_config.cpp      path_security.cpp		 MethodAi.cpp redir_path.cpp \
isCgiPath.cpp MethodCgi.cpp \
FieldDelete.cpp					delete_f.cpp 					FieldPost.cpp					date_string.cpp					is_not_number.cpp				trim_fields.cpp 				remove_chars_after_the_last_token.cpp decompose_path.cpp \
MethodPost.cpp					extract_header_request.cpp		ChunkedRequest.cpp				atoi_hexa.cpp					MethodPostCgi.cpp 				fields_allowed.cpp create_tmp_file.cpp \
upload_utilitaries.cpp MethodUpload.cpp


PATH_OBJS = ./objs/
OBJS = $(addprefix ${PATH_OBJS},${SRCS:.cpp=.o})
OBJS_CONFIG = $(addprefix ,${CONFIG:.cpp=.o})

DEBUG = -g  #-fsanitize=address -fsanitize=undefined

CXX= clang++

INCLUDES= -I./srcs/methods -I./srcs -I/includes

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Weffc++

RM = rm -f

$(PATH_OBJS)%.o :%.cpp
	$(CXX) $(CXXFLAGS) ${DEBUG}  -c $< -o $@


all : 		${PATH_OBJS} ${NAME} 


${NAME}:	${OBJS} ${OBJS_CONFIG}
			$(CXX) ${CXXFLAGS} ${DEBUG}   ${OBJS}  -o $@

clean :		
			${RM} ${OBJS}

fclean :	clean
			${RM} ${NAME}

re :		fclean all

${PATH_OBJS}:
	mkdir -p $@

.PHONY : all clean fclean re
