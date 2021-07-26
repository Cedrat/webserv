NAME = webserv

PATH_SRC = ./srcs/
SRCS = main.cpp Config.cpp Location.cpp Server.cpp Socket.cpp regexp.cpp  Request.cpp count_words.cpp response_error_header.cpp error_tables.cpp
PATH_OBJS = ./objs/
OBJS = $(addprefix ${PATH_SRC},${SRCS:.cpp=.o})

CXX= clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

%.o :%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

all : 		${NAME}

${NAME}:	${OBJS}
			$(CXX) ${CFLAGS} ${OBJS} -o $@

clean :		
			${RM} ${OBJS}

fclean :	clean
			${RM} ${NAME}

re :		fclean all

.PHONY : all clean fclean re
