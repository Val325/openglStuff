OBJS = triangular.cpp includes/src/glad.c
CFLAG = -Wall -g
CC = g++
INCLUDES = -Iincludes -Iincludes/include
LIBS = -Llib -lglfw -lGL -lglut -lGLEW  -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp
prog:${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}
