CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-error=unused-parameter `sdl-config --cflags` -g -I. -I.. -I../game/include
LDLIBS = -lSDL -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL_gfx -lm

# Directories
SRC_DIR = .
GAME_DIR = ../game/include
MENU_SAVE_DIR = menu_save
MENU_BEST_SCORE_DIR = menu_best_score
MENU_PLAYER_DIR = menu_joueur
MENU_PRINCIPAL_DIR = menu_principal
MENU_OPTION_DIR = menu_option
MENU_ENIGME_DIR = menu_enigme
BIN_DIR = ../bin
OBJ_DIR = obj

# Source files
SRCS = $(SRC_DIR)/menu.c \
       $(MENU_SAVE_DIR)/menuSave.c \
       $(MENU_BEST_SCORE_DIR)/menuBestScore.c \
	   $(MENU_PLAYER_DIR)/menuJoueur.c \
	   $(MENU_PRINCIPAL_DIR)/menuPrincipal.c \
	   $(MENU_OPTION_DIR)/menuOption.c \
       $(MENU_ENIGME_DIR)/menuEnigme.c

HELPERS_SRC = ../helpers.c

# Object files
OBJS = $(SRCS:.c=.o)
HELPERS_OBJ = $(OBJ_DIR)/helpers.o

# Executable
TARGET = $(BIN_DIR)/menu

all: $(OBJS) $(HELPERS_OBJ)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(HELPERS_OBJ): $(HELPERS_SRC) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MENU_ENIGME_DIR)/menuEnigme.o: $(MENU_ENIGME_DIR)/menuEnigme.c $(MENU_ENIGME_DIR)/menuEnigme.h header.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MENU_SAVE_DIR)/menuSave.o: $(MENU_SAVE_DIR)/menuSave.c $(MENU_SAVE_DIR)/menuSave.h header.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MENU_BEST_SCORE_DIR)/menuBestScore.o: $(MENU_BEST_SCORE_DIR)/menuBestScore.c $(MENU_BEST_SCORE_DIR)/menuBestScore.h header.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MENU_PLAYER_DIR)/menuJoueur.o: $(MENU_PLAYER_DIR)/menuJoueur.c $(MENU_PLAYER_DIR)/menuJoueur.h header.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MENU_PRINCIPAL_DIR)/menuPrincipal.o: $(MENU_PRINCIPAL_DIR)/menuPrincipal.c $(MENU_PRINCIPAL_DIR)/menuPrincipal.h header.h
	$(CC) $(CFLAGS) -c $< -o $@

$(MENU_OPTION_DIR)/menuOption.o: $(MENU_OPTION_DIR)/menuOption.c $(MENU_OPTION_DIR)/menuOption.h header.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/menu.o: $(SRC_DIR)/menu.c $(SRC_DIR)/header.h ../helpers.h $(MENU_SAVE_DIR)/menuSave.h $(MENU_BEST_SCORE_DIR)/menuBestScore.h $(MENU_PLAYER_DIR)/menuJoueur.h $(MENU_PRINCIPAL_DIR)/menuPrincipal.h $(MENU_OPTION_DIR)/menuOption.h $(MENU_ENIGME_DIR)/menuEnigme.h $(GAME_DIR)/game.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(HELPERS_OBJ)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all


