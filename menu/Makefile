CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-error=unused-parameter `sdl-config --cflags` -g
LDLIBS = `sdl-config --libs` -lSDL_mixer -lSDL_ttf -lSDL_image

# Directories
SRC_DIR = .
MENU_SAVE_DIR = menu_save
MENU_BEST_SCORE_DIR = menu_best_score
MENU_PLAYER_DIR = menu_joueur
MENU_PRINCIPAL_DIR = menu_principal
MENU_OPTION_DIR = menu_option
MENU_ENIGME_DIR = menu_enigme
BIN_DIR = ../bin

# Source files
SRCS = $(SRC_DIR)/menu.c \
       $(MENU_SAVE_DIR)/menuSave.c \
       $(MENU_BEST_SCORE_DIR)/menuBestScore.c \
       $(MENU_PLAYER_DIR)/menuJoueur.c \
       $(MENU_PRINCIPAL_DIR)/menuPrincipal.c \
       $(MENU_OPTION_DIR)/menuOption.c \
       $(MENU_ENIGME_DIR)/menuEnigme.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = $(BIN_DIR)/menu

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/menu.o: $(SRC_DIR)/menu.c $(MENU_SAVE_DIR)/menuSave.h $(MENU_BEST_SCORE_DIR)/menuBestScore.h $(MENU_PLAYER_DIR)/menuJoueur.h $(MENU_PRINCIPAL_DIR)/menuPrincipal.h $(MENU_OPTION_DIR)/menuOption.h $(MENU_ENIGME_DIR)/menuEnigme.h $(SRC_DIR)/header.h

clean:
	rm -f $(OBJS) $(TARGET)

re: clean all


