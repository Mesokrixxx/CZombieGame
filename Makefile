NAME = ZombieGame

CC = cc
CFLAGS = -I$(INCLUDE)
LFLAGS = -lSDL2 -lGLEW -lGL

INCLUDE = include
SRCS = $(wildcard src/*.c src/**/*.c)
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(NAME) $(LFLAGS)

clean:
	rm -f $(OBJS)
	rm -f $(NAME)

re: clean all

.PHONY: all clean re
