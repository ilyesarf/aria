game: enemymain.o enemy.o
	gcc enemymain.o enemy.o -o game -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lm -g

enemymain.o: enemymain.c enemy.h
	gcc -c enemymain.c -g

enemy.o: enemy.c enemy.h
	gcc -c enemy.c -g

clean:
	rm -f *.o game

.PHONY: clean