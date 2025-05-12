CC = gcc
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags sdl SDL_image SDL_ttf SDL_mixer) -I./core
LDFLAGS = $(shell pkg-config --libs sdl SDL_image SDL_ttf SDL_mixer) -lm

SRCDIR = core
OBJDIR = obj

# Main source files
SOURCES = $(SRCDIR)/main.c \
         $(SRCDIR)/background/bgmain.c \
         $(SRCDIR)/background/bgsource.c

OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

TARGET = core/game

$(TARGET): $(OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: run
run: $(TARGET)
	./$(TARGET) 