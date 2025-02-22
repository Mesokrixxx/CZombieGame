NAME = ZombieGame

# Détection de l'OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    NAME := $(NAME).exe
    CC := cc
    DEFAULT_LIB_DIR = C:/w64devkit
    CFLAGS = -I$(INCLUDE_DIR) -I$(LIB_DIR)/src/include -I$(DEFAULT_LIB_DIR)/include -g3
    LFLAGS = -lmingw32 -L$(LIB_DIR) -lGE -lglew32 -lopengl32 -lSDL2main -lSDL2
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macOS
        CC := clang
        FRAMEWORK = -framework OpenGL -framework Cocoa
    else
        DETECTED_OS := Linux
        CC := cc
    endif
    LFLAGS = -L$(LIB_DIR) -lGE -lGLEW -lSDL2 -lGL
    CFLAGS = -I$(INCLUDE_DIR) -I${LIB_DIR}/src/include -g
endif

LIB_DIR = libGE

INCLUDE_DIR = src/include/
SRCS = $(wildcard src/*.c src/**/*.c)
OBJ = $(SRCS:.c=.o)

all: lib game

lib: 
	cd $(LIB_DIR) && make

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME) $(LFLAGS)

clean:
	rm -f $(OBJ)
	rm -f $(NAME)
	cd $(LIB_DIR) && make clean

re: clean all

game:
	rm -f $(NAME)
	$(MAKE) $(NAME)

.PHONY: all clean re lib game
