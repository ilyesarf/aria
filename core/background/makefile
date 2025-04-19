CC = gcc
CFLAGS = -Wall -g `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_image
TARGET = bgtest
SRCS = bgmain.c background.c

all:
    $(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
    rm -f $(TARGET)