NAME = ZombieGame

# mingw et sdl2 pour windows 
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    NAME := $(NAME).exe
    CC := gcc
    LFLAGS = -L$(LIB_DIR) -lGE -lmingw32 -lSDL2main -lSDL2
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macOS
        CC := clang
        LFLAGS = -L$(LIB_DIR) -lGE -lSDL2
        FRAMEWORK = -framework OpenGL -framework Cocoa
    else
        DETECTED_OS := Linux
        CC := cc
        LFLAGS = -L$(LIB_DIR) -lGE -lSDL2
    endif
endif

CFLAGS = -Wall -Werror -Wextra -I${INCLUDE_DIR} -I${LIB_DIR_INCLUDE}

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
