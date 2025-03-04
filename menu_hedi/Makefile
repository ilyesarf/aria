prog: main.o funct.o
	gcc main.o funct.o -o prog -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -g

main.o: main.c main.h
	gcc -c main.c -g

funct.o: funct.c main.h
	gcc -c funct.c -g

clean:
	rm -f *.o prog

.PHONY: clean
