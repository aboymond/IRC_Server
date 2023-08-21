NAME = ircserv
SRCS = main.cpp server.cpp client.cpp
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++98 #-fsanitize=address
OBJ = ${SRCS:.cpp=.o}
# fin de la declatation des variables

all : ${NAME}

${NAME} : ${OBJ}
	${CXX} ${CXXFLAGS} ${OBJ} -o ${NAME}

clean :
	rm -rf *.o

fclean : clean
	rm -rf ${NAME}

re : fclean all