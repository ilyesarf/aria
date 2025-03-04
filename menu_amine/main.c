#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "option.h"
#include "joueur.h"
#include "principal.h"

int main(int argc, char** argv) {
    SDL_Surface *ecran = NULL, *image = NULL;
    int quitter = 1;
    SDL_Rect posecranimg = {0, 0};
    SDL_Color textColor = {0, 0, 0}; // white color
    TTF_Font *font = NULL, *windowModeFont = NULL; // Separate font for "Window Mode" text
    Mix_Music *musique = NULL;
    SDL_Event event;
    int volume = 50; // Initial volume (50%)
    int showWindowModeText = 1; // Flag to show "Window Mode" text
    Button buttons[4];
    int menustate = MENU_OPTIONS; // Start with the main menu

    // Initialize SDL
    if (init_sdl()) {
        return 1;
    }

    // Create a window
    ecran = create_window(1792, 1024, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!ecran) {
        return 1;
    }

    // Load images
    image = load_image("222222_1.jpg");
    if (!image ) {
        return 0;
    }

    // Set image position
    posecranimg.x = 0;
    posecranimg.y = 0;
    posecranimg.w = image->w;
    posecranimg.h = image->h;

    // Load fonts
    font = load_font("Pixelon-OGALo.ttf", 20);
    windowModeFont = load_font("Pixelon-OGALo.ttf", 40); // Larger font for "Window Mode" text

    // Load music
    musique = load_music("30-hours.mp3");
    if (!musique ) {
        return 0;
    }

    // Play music
    Mix_PlayMusic(musique, -1);
    Mix_VolumeMusic(volume); // Set initial volume

    // Create buttons
    create_buttons(buttons);

    // Main game loop
    while (quitter) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        quitter = 0;
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // Check if any button was clicked
                        for (int i = 0; i < 5; i++) {
                            if (is_mouse_over_button(&buttons[i], event.button.x, event.button.y)) {
                                handle_button_click(&buttons[i], &quitter, &volume, ecran, &showWindowModeText, &menustate);
                            }
                        }
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    break;

                case SDL_MOUSEMOTION:
                    // Update button states for all buttons
                    for (int i = 0; i < 5; i++) {
                        update_button_state(&buttons[i], event.motion.x, event.motion.y);
                    }
                    break;
            }
        }

        // Render the appropriate menu based on the menustate variable
        if (menustate == MENU_MAIN) {
            principal_menu(ecran, musique, &menustate, image);
        } else if (menustate == MENU_OPTIONS) {
            option(ecran, image, posecranimg, buttons, font, windowModeFont, musique, &quitter, &volume, &showWindowModeText, textColor, &menustate);
        } else if (menustate == MENU_JOUEUR) {
            joueur(ecran, image, musique, font, textColor, &menustate);
        } else if (menustate == -1) {
            quitter = 0; // Quit the game
        }
    }

    // Cleanup resources
    cleanup_resources(image, musique, font, buttons, 5);
    if (image) SDL_FreeSurface(image);
    if (musique) Mix_FreeMusic(musique);
    TTF_CloseFont(windowModeFont); // Cleanup the larger font

    return 0;
}