#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "principal.h"
#include "option.h"
#include <stdio.h>

void principal_menu(SDL_Surface* ecran, Mix_Music* musique, int* menustate, SDL_Surface* image) {
    Button buttons[4];
    SDL_Event event;
    int loop = 1;

    // Load images for buttons
    buttons[0] = create_button(700, 300, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1hov.png");
    buttons[1] = create_button(700, 450, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1hov.png");
    buttons[2] = create_button(700, 600, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1hov.png");
    buttons[3] = create_button(700, 750, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/return.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/returnhov.png");

    // Check if all button images are loaded correctly
    for (int i = 0; i < 4; i++) {
        if (!buttons[i].normalImage || !buttons[i].hoverImage) {
            printf("Failed to load images for button %d\n", i);
            return;
        }
    }

    while (loop) {
        SDL_BlitSurface(image, NULL, ecran, NULL);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    loop = 0;
                    *menustate = -1; // Indicate quitting the game
                    break;

                case SDL_MOUSEMOTION:
                    for (int i = 0; i < 4; i++) {
                        update_button_state(&buttons[i], event.motion.x, event.motion.y);
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        for (int i = 0; i < 4; i++) {
                            if (is_mouse_over_button(&buttons[i], event.button.x, event.button.y)) {
                                if (i == 0) { // First button
                                    printf("First button clicked!\n");
                                    // Handle first button click
                                } else if (i == 1) { // Options button
                                    *menustate = MENU_OPTIONS;
                                    printf("Options button clicked!\n");
                                    loop = 0;
                                } else if (i == 2) { // Joueur button
                                    *menustate = MENU_JOUEUR;
                                    printf("Joueur button clicked!\n");
                                    loop = 0;
                                } else if (i == 3) { // Quit button
                                    loop = 0;
                                    *menustate = -1; // Indicate quitting the game
                                    printf("Quit button clicked!\n");
                                }
                            }
                        }
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            loop = 0;
                            *menustate = -1; // Indicate quitting the game
                            break;
                    }
                    break;
            }
        }

        for (int i = 0; i < 4; i++) {
            render_button(ecran, &buttons[i]);
        }

        SDL_Flip(ecran);
    }

    // Cleanup resources
    for (int i = 0; i < 4; i++) {
        SDL_FreeSurface(buttons[i].normalImage);
        SDL_FreeSurface(buttons[i].hoverImage);
    }
}