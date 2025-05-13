CC = gcc
CFLAGS = -Wall -Wextra -Werror -Wno-error=unused-parameter `sdl-config --cflags` -g -I. -I.. -I../game/include
LDLIBS = -lSDL -lSDL_mixer -lSDL_ttf -lSDL_image -lSDL_gfx -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
MENU_DIR = ../menu
BIN_DIR = ../bin
OBJ_DIR = obj

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
HELPERS_SRC = ../helpers.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
HELPERS_OBJ = $(OBJ_DIR)/helpers.o

# Menu objects (these will be built by menu's Makefile)
MENU_OBJS = $(MENU_DIR)/menu.o \
            $(MENU_DIR)/menu_save/menuSave.o \
            $(MENU_DIR)/menu_best_score/menuBestScore.o \
            $(MENU_DIR)/menu_joueur/menuJoueur.o \
            $(MENU_DIR)/menu_principal/menuPrincipal.o \
            $(MENU_DIR)/menu_option/menuOption.o \
            $(MENU_DIR)/menu_enigme/menuEnigme.o

# Executable
TARGET = $(BIN_DIR)/game

all: menu_compile $(TARGET)

menu_compile:
	$(MAKE) -C $(MENU_DIR)

$(TARGET): $(OBJS) $(HELPERS_OBJ) $(MENU_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(MENU_DIR) -c $< -o $@

$(HELPERS_OBJ): $(HELPERS_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(MENU_DIR) clean
	rm -f $(OBJS) $(HELPERS_OBJ)

fclean: clean
	$(MAKE) -C $(MENU_DIR) fclean
	rm -f $(TARGET)
	rm -rf $(OBJ_DIR)

re: fclean all 