prog:main.o time.o map.o pres.o
	gcc main.o time.o map.o pres.o -o prog -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
main.o:main.c
	gcc -c main.c -g
time.o:time.c 
	gcc -c time.c -g
map.o:map.c
	gcc -c map.c -g 
pres.o:pres.c
	gcc -c pres.c -g
clavier.o:clavier.c
	gcc -c clavier.c -g

