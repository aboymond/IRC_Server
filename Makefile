NAME = ircserv
SRCS = User.cpp
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++98
SANITIZE = -fsanitize=address -fno-omit-frame-pointer
OBJ = ${SRCS:.cpp=.o}
ARGS =	6667 test
# fin de la declatation des variables

all : ${NAME}

${NAME} : ${OBJ}
	${CXX} ${CXXFLAGS} ${OBJ} -o ${NAME}

l : ${OBJ}
	${CXX} ${CXXFLAGS} ${SANITIZE} ${OBJ} -o ${NAME}

clean :
	rm -rf *.o

fclean : clean
	rm -rf ${NAME}

re : fclean all

re_l : fclean l

run : re
	./${NAME} ${ARGS}

run_l : re_l
	./${NAME} ${ARGS}