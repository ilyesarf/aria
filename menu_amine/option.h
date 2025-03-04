#ifndef OPTION_H_INCLUDED
#define OPTION_H_INCLUDED

#define MENU_MAIN 1
#define MENU_OPTIONS 2
#define MENU_JOUEUR 3

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

// Function prototypes
typedef struct {
    SDL_Rect position; // Position and size of the button
    SDL_Surface* normalImage; // Image for the normal state
    SDL_Surface* hoverImage;  // Image for the hover state
    int isActive;             // Indicates if the button is currently active (hovered)
} Button;

// Function prototypes
int init_sdl();
SDL_Surface* create_window(int width, int height, int bpp, Uint32 flags);
SDL_Surface* load_image(const char* filename);
Mix_Music* load_music(const char* filename);
TTF_Font* load_font(const char* filename, int size);
void create_buttons(Button* buttons);
Button create_button(int x, int y, int w, int h, const char* normalPath, const char* hoverPath);
void update_button_state(Button* button, int mouseX, int mouseY);
void render_button(SDL_Surface* screen, Button* button);
void render_text(SDL_Surface* screen, const char* text, TTF_Font* font, SDL_Color color, SDL_Rect position);
void handle_button_click(Button* button, int* quitter, int* volume, SDL_Surface* ecran, int* showWindowModeText, int* menustate);
void cleanup_resources(SDL_Surface* image, Mix_Music* music, TTF_Font* font, Button* buttons, int buttonCount);
int is_mouse_over_button(Button* button, int mouseX, int mouseY);
void toggle_fullscreen(SDL_Surface* screen);
void option(SDL_Surface* ecran, SDL_Surface* image, SDL_Rect posecranimg, Button* buttons, TTF_Font* font, TTF_Font* windowModeFont, Mix_Music* musique, int* quitter, int* volume, int* showWindowModeText, SDL_Color textColor, int* menustate);
void joueur(SDL_Surface* ecran, SDL_Surface* background2, Mix_Music* music, TTF_Font* font, SDL_Color textColor,int* menustate);

#endif // OPTION_H_INCLUDED