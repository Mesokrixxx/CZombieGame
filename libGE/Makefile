NAME = libGE.a

# Détection du système d'exploitation
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    CC := gcc
    LFLAGS = -lmingw32 -lSDL2main -lSDL2
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macOS
        CC := clang
        LFLAGS = -lSDL2
        FRAMEWORK = -framework OpenGL -framework Cocoa
    else
        DETECTED_OS := Linux
        CC := cc
        LFLAGS = -lSDL2
    endif
endif

CFLAGS = -Wall -Werror -Wextra -I${INCLUDE_DIR}

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
