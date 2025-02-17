NAME = ZombieGame

# Détection de l'OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    NAME := $(NAME).exe
    CC := gcc
    SDL2_DIR = C:/w64devkit
    CFLAGS = -Wall -Werror -Wextra -I$(INCLUDE_DIR) -I$(LIB_DIR)/src/include -I$(SDL2_DIR)/include
    LFLAGS = -L$(LIB_DIR) -lGE -L$(SDL2_DIR)/lib -lmingw32 -lSDL2main -lSDL2
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
    CFLAGS = -Wall -Werror -Wextra -I$(INCLUDE_DIR) -I${LIB_DIR}/include
endif

LIB_DIR = libGE

INCLUDE_DIR = src/include/
SRCS = $(wildcard src/*.c src/**/*.c)
OBJ = $(SRCS:.c=.o)

all: lib $(NAME)

debug: clean lib_debug $(NAME)

lib_debug: 
	cd $(LIB_DIR) && make debug

lib: 
	cd $(LIB_DIR) && make

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(NAME)
	cd $(LIB_DIR) && make clean

re: clean all

.PHONY: all clean re
