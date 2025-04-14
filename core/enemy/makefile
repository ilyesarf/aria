CC = gcc
CFLAGS = -Wall `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_image
SRC = enemymain.c enemy.c
OBJ = $(SRC:.c=.o)
EXEC = enemy_game

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o $(EXEC)
