NAME = libGE.a

CC = cc
CFLAGS = -Wall -Werror -Wextra -I${INCLUDE_DIR}
LFLAGS = -lSDL2

INCLUDE_DIR = src/include/
SRCS = $(shell find src -name '*.c')
OBJ = $(SRCS:.c=.o)

all: ${NAME}

debug:
	${MAKE} "CFLAGS = ${CFLAGS} -DDEBUG" 

${NAME}: ${OBJ}
	ar crs ${NAME} ${OBJ}

clean:
	rm -f ${OBJ}
	rm -f ${NAME}

re: clean all

.PHONY: all clean re
