NAME = webserv

PATH_SRC = ./srcs/
SRCS = main.cpp Config.cpp Location.cpp Server.cpp Socket.cpp regexp.cpp  Request.cpp count_words.cpp response_error_header.cpp error_tables.cpp \
		string_manip.cpp request_conditions.cpp path_security.cpp delete_and_give_response.cpp delete_f.cpp create_ai.cpp response_post.cpp \
		create_path.cpp
PATH_OBJS = ./objs/
OBJS = $(addprefix ${PATH_SRC},${SRCS:.cpp=.o})
DEBUG = -g3 -fsanitize=address

CXX= clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

%.o :%.cpp
	$(CXX) $(CFLAGS) ${DEBUG} -c $< -o $@

all : 		${NAME}


${NAME}:	${OBJS}
			$(CXX) ${CFLAGS} ${DEBUG} ${OBJS} -o $@

clean :		
			${RM} ${OBJS}

fclean :	clean
			${RM} ${NAME}

re :		fclean all

.PHONY : all clean fclean re
