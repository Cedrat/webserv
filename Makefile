NAME = webserv

PATH_SRC = ./srcs/
SRCS = main_test.cpp PollSocket.cpp Server.cpp Socket.cpp SocketClient.cpp

PATH_OBJS = ./objs/
OBJS = $(addprefix ${PATH_SRC},${SRCS:.cpp=.o})

CXX= clang++

CFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -f

${PATH_OBJS}%.o :.cpp
	$(CXX) $(CFLAGS) -c $< 

all : 		${NAME}

${NAME}:	${OBJS}
			$(CXX) ${CFLAGS} ${OBJS} -o $@

clean :		
			${RM} ${OBJS}

fclean :	clean
			${RM} ${NAME}

re :		fclean all

.PHONY : all clean fclean re
