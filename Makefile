NAME = webserv

PATH_SRC = .
SRCS = main_test.cpp PollSocket.cpp Server.cpp Socket.cpp SocketClient.cpp

OBJS = ${PATH_SOURCE}${SRCS:.cpp=.o}

CXX= clang++

CFLAGS = -Wall -Wextra -Werror

RM = rm -f

%.o : %.cpp
	$(CXX) -c $(CFLAGS) -std=c++98 $< -o $@

all : 		${NAME}

${NAME}:	${OBJS}
			clang++ -std=c++98 -o ${NAME} ${CFLAGS} ${SRCS}

clean :		
			${RM} ${OBJS} ${BONUS}

fclean :	clean
			${RM} ${NAME}

re :		fclean all