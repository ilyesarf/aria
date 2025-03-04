prog: main.o option.o menujoueur.o principal.o
	gcc main.o option.o menujoueur.o principal.o -o prog -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -g

main.o: main.c option.h joueur.h principal.h
	gcc -c main.c -g

option.o: option.c option.h
	gcc -c option.c -g

menujoueur.o: menujoueur.c option.h joueur.h
	gcc -c menujoueur.c -g

principal.o: principal.c principal.h
	gcc -c principal.c -g

clean:
	rm -f *.o prog

.PHONY: clean
