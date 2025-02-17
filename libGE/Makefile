NAME = libGE.a

# Détection de l'OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    CC := gcc
    SDL_INCLUDE = C:/w64devkit/include/
    LFLAGS = -lmingw32 -lSDL2main -lSDL2
    CFLAGS = -Wall -Werror -Wextra -I$(INCLUDE_DIR) -I${SDL_INCLUDE}
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
    CFLAGS = -Wall -Werror -Wextra -I$(INCLUDE_DIR)
endif


INCLUDE_DIR = src/include/
SRCS = $(wildcard src/*.c src/**/*.c)
OBJ = $(SRCS:.c=.o)

all: $(NAME)

debug: CFLAGS += -DDEBUG
debug: clean all

$(NAME): $(OBJ)
	ar crs $(NAME) $(OBJ)

clean:
	rm -f $(OBJ) $(NAME)

re: clean all

.PHONY: all clean re debug
