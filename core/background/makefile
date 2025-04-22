CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-error=unused-parameter `sdl-config --cflags` -g
LDLIBS = -lSDL -lSDL_image

# Directories
SRC_DIR = .
BIN_DIR = ../../bin

# Source files
SRCS = $(SRC_DIR)/bgmain.c \
	   $(SRC_DIR)/bgsource.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = $(BIN_DIR)/bgtest

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)  # <-- This line starts with a tab

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@  # <-- This line starts with a tab

clean:
	rm -f $(OBJS) $(TARGET)  # <-- This line starts with a tab

re: clean all