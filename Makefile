INCLUDE = include
SRCS = $(wildcard src/*.c src/**/*.c)
OBJS = $(SRCS:.c=.o)

ifeq ($(OS), Windows_NT)
	NAME = ZombieGame.exe
	
	RM = del
	
	CC = cc
	W64PATH = C:/w64devkit
	CFLAGS = -I$(INCLUDE) -I$(W64PATH)/include
	LFLAGS = -L$(W64PATH)/lib -lmingw32 -lSDL2main -lSDL2 -lglew32 -lopengl32
else
	NAME = ZombieGame

	RM = rm -f 
	
	CC = cc
	CFLAGS = -I$(INCLUDE)
	LFLAGS = -lSDL2 -lGLEW -lGL
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME) $(LFLAGS)

clean:
	$(RM) -f $(OBJS)
	$(RM) -f $(NAME)

re: clean all

.PHONY: all clean re
