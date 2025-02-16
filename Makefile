NAME = ZombieGame

CC = cc
CFLAGS = -Wall -Werror -Wextra -I${INCLUDE_DIR} -I${LIB_DIR_INCLUDE}
LFLAGS = -L${LIB_DIR} -lGE -lSDL2

LIB_DIR = libGE/
LIB_DIR_INCLUDE = ${LIB_DIR}src/include/

INCLUDE_DIR = src/include/
SRCS = $(shell find src -name '*.c')
OBJ = $(SRCS:.c=.o)

all: lib ${NAME}

debug: clean lib_debug ${NAME}

lib_debug: 
	cd ${LIB_DIR} && make debug

lib: 
	cd ${LIB_DIR} && make

${NAME}: ${OBJ}
	${CC} ${OBJ} ${CFLAGS} -o ${NAME} ${LFLAGS}

clean:
	rm -f ${OBJ}
	rm -f ${NAME}
	cd ${LIB_DIR} && make clean

re: clean all

.PHONY: all clean re
