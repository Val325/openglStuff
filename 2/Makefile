OBJS = main.cpp /home/projects/opengl/1/includes/src/glad.c
CFLAG = -Wall -g
CC = g++
INCLUDES = -I/home/projects/opengl/1/includes/include -I/home/projects/opengl/1/includes
LIBS = -lglfw -lGL -lglut -lGLEW  -lGL -lX11 -lpthread -lXrandr -lXi -ldl

compile:${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f *.o core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<
