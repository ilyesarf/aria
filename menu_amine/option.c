#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "option.h"
#include <stdio.h>

// Initialize SDL subsystems
int init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Failed to initialize SDL_mixer: %s\n", Mix_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        return 1;
    }
    return 0;
}

// Create a window
SDL_Surface* create_window(int width, int height, int bpp, Uint32 flags) {
    SDL_Surface* screen = SDL_SetVideoMode(width, height, bpp, flags);
    if (!screen) {
        fprintf(stderr, "Failed to create window: %s.\n", SDL_GetError());
    }
    return screen;
}

// Load an image
SDL_Surface* load_image(const char* filename) {
    SDL_Surface* loadedImage = IMG_Load(filename);
    if (!loadedImage) {
        printf("Failed to load image '%s': %s\n", filename, IMG_GetError());
    }
    return loadedImage;
}

// Load music
Mix_Music* load_music(const char* filename) {
    Mix_Music* music = Mix_LoadMUS(filename);
    if (!music) {
        printf("Failed to load music '%s': %s\n", filename, Mix_GetError());
    }
    return music;
}

// Load a font
TTF_Font* load_font(const char* filename, int size) {
    TTF_Font* font = TTF_OpenFont(filename, size);
    if (!font) {
        printf("Failed to load font '%s': %s\n", filename, TTF_GetError());
    }
    return font;
}

// Create buttons
void create_buttons(Button* buttons) {
    buttons[0] = create_button(400, 185, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/Fullscreen.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/Fullscreenhov.png");
    buttons[1] = create_button(850, 185, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/windowed.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/windowedhov.png");
    buttons[2] = create_button(150, 650, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/return.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/returnhov.png");
    buttons[3] = create_button(400, 350, 50, 40,  "/home/amine/pizza/SDL_Workshop_Student/buttons/volumeup.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/volumeuphov.png");
    buttons[4] = create_button(850, 350, 50, 40,  "/home/amine/pizza/SDL_Workshop_Student/buttons/volumedwn.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/volumedwnhov.png");
}

// Create a button
Button create_button(int x, int y, int w, int h, const char* normalPath, const char* hoverPath) {
    Button button;
    button.position.x = x;
    button.position.y = y;
    button.position.w = w;
    button.position.h = h;
    button.normalImage = IMG_Load(normalPath);
    if (!button.normalImage) {
        printf("Failed to load normal image '%s': %s\n", normalPath, IMG_GetError());
    }
    button.hoverImage = IMG_Load(hoverPath);
    if (!button.hoverImage) {
        printf("Failed to load hover image '%s': %s\n", hoverPath, IMG_GetError());
    }
    button.isActive = 0;
    return button;
}
// Check if the mouse is over a button
int is_mouse_over_button(Button* button, int mouseX, int mouseY) {
    return (mouseX >= button->position.x && mouseX <= button->position.x + button->position.w &&
            mouseY >= button->position.y && mouseY <= button->position.y + button->position.h);
}

// Update button state based on mouse position
void update_button_state(Button* button, int mouseX, int mouseY) {
    button->isActive = is_mouse_over_button(button, mouseX, mouseY);
}

// Render a button
void render_button(SDL_Surface* screen, Button* button) {
    SDL_Surface* image;
    if (button->isActive) {
        image = button->hoverImage;
    } else {
        image = button->normalImage;
    }

    if (!image) {
        printf("Error rendering button: SDL_UpperBlit: passed a surface with NULL pixels\n");
        return;
    }

    SDL_BlitSurface(image, NULL, screen, &button->position);
}

// Render text on the screen
void render_text(SDL_Surface* screen, const char* text, TTF_Font* font, SDL_Color color, SDL_Rect position) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) {
        printf("Failed to render text: %s\n", TTF_GetError());
        return;
    }
    SDL_BlitSurface(textSurface, NULL, screen, &position);
    SDL_FreeSurface(textSurface); // Free the text surface after rendering
}

// Handle button click
void handle_button_click(Button* button, int* quitter, int* volume, SDL_Surface* ecran, int* showWindowModeText,int* menustate) {
    if (button->position.x == 400 && button->position.y == 185) {
        printf("Fullscreen button clicked!\n");
        if (!(ecran->flags & SDL_FULLSCREEN)) {
            toggle_fullscreen(ecran);
            *showWindowModeText = 1; // Show "Window Mode" text
        }
    } else if (button->position.x == 850 && button->position.y == 185) {
        printf("Windowed button clicked!\n");
        if (ecran->flags & SDL_FULLSCREEN) {
            toggle_fullscreen(ecran);
            *showWindowModeText = 1; // Show "Window Mode" text
        }
    } else if (button->position.x == 150 && button->position.y == 650) {
        printf("Quit button clicked!\n");
        //*quitter = 0; // Exit the program
        *menustate=1;
    } else if (button->position.x == 400 && button->position.y == 350) {
        printf("Volume Up button clicked!\n");
        *volume = (*volume + 5 > 100) ? 100 : *volume + 5;
        Mix_VolumeMusic(*volume);
    } else if (button->position.x == 850 && button->position.y == 350) {
        printf("Volume Down button clicked!\n");
        *volume = (*volume - 5 < 0) ? 0 : *volume - 5;
        Mix_VolumeMusic(*volume);
    }
}

// Toggle fullscreen/windowed mode
void toggle_fullscreen(SDL_Surface* screen) {
    if (SDL_WM_ToggleFullScreen(screen) == 0) {
        printf("Failed to toggle fullscreen: %s\n", SDL_GetError());
    }
}

// Main game loop function
void option(SDL_Surface* ecran, SDL_Surface* image, SDL_Rect posecranimg, Button* buttons, TTF_Font* font, TTF_Font* windowModeFont, Mix_Music* musique, int* quitter, int* volume, int* showWindowModeText, SDL_Color textColor, int* menustate) {
    SDL_Rect windowModeTextPos = {150, 250}, volumeLevelTextPos = {150, 400}; // Positions for texts
    char volumeLevelText[50]; // Buffer to hold volume level text
    // Render background
    SDL_BlitSurface(image, &posecranimg, ecran, NULL);

    // Render buttons
    for (int i = 0; i < 5; i++) {
        render_button(ecran, &buttons[i]);
    }

    // Render "Window Mode" text if the flag is set
    if (*showWindowModeText) {
        render_text(ecran, "Window Mode", windowModeFont, textColor, windowModeTextPos);
    }

    // Render current volume level text
    snprintf(volumeLevelText, sizeof(volumeLevelText), "Volume: %d", *volume);
    render_text(ecran, volumeLevelText, windowModeFont, textColor, volumeLevelTextPos);

    SDL_Flip(ecran); // Update screen
}

// Cleanup resources
void cleanup_resources(SDL_Surface* image, Mix_Music* music, TTF_Font* font, Button* buttons, int buttonCount) {
    if (image) SDL_FreeSurface(image);
    if (music) Mix_FreeMusic(music);
    if (font) TTF_CloseFont(font);
    for (int i = 0; i < buttonCount; i++) {
        if (buttons[i].normalImage) SDL_FreeSurface(buttons[i].normalImage);
        if (buttons[i].hoverImage) SDL_FreeSurface(buttons[i].hoverImage);
    }
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}