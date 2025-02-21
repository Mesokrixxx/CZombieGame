NAME = libGE.a

# Détection de l'OS
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    CC := cc
    DEFAULT_LIB_DIR = C:/w64devkit
    CFLAGS = -I$(INCLUDE_DIR) -I${DEFAULT_LIB_DIR}/include
    LFLAGS = -lmingw32 -lglew32 -lopengl32 -lSDL2main -lSDL2
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
    LFLAGS = -L$(LIB_DIR) -lGE -lGLEW -lSDL2 
    CFLAGS = -I$(INCLUDE_DIR)
endif

INCLUDE_DIR = src/include/
SRCS = $(wildcard src/*.c src/**/*.c)
OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar crs $(NAME) $(OBJ)

clean:
	rm -f $(OBJ) $(NAME)

re: clean all

.PHONY: all clean re
