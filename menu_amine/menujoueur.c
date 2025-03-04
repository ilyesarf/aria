#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "option.h"
#include "joueur.h"
#include <stdio.h>

void joueur(SDL_Surface* ecran, SDL_Surface* image, Mix_Music* musique, TTF_Font* font, SDL_Color textColor, int* menustate) {
    int quitter = 1;
    SDL_Event event;
    Button buttons[9];
    int showSubButtons = 0;


    // Create main buttons
    buttons[0] = create_button(400, 300, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/singleplayer.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/singleplayerhov.png");
    buttons[1] = create_button(1000, 300, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/Multiplayer.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/Multiplayerhov.png");
    buttons[2] = create_button(1100, 850, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/return.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/returnhov.png");
    // Create sub buttons
    buttons[3] = create_button(400, 185, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar1hov.png");
    buttons[4] = create_button(1000, 185, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar2.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/avatar2hov.png");
    buttons[5] = create_button(400, 400, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/input1.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/input1hov.png");
    buttons[6] = create_button(1000, 400, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/input2.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/input2hov.png");
    buttons[7] = create_button(700, 600, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/submit.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/submithov.png");
    buttons[8] = create_button(1400, 850, 150, 40, "/home/amine/pizza/SDL_Workshop_Student/buttons/return.png", "/home/amine/pizza/SDL_Workshop_Student/buttons/returnhov.png");

    while (quitter) {
        // Render background
        SDL_BlitSurface(image, NULL, ecran, NULL);
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quitter = 0;
                    *menustate = -1; 
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        printf("Enter key pressed!\n");
                        // Handle Enter key press to show the "best scores" submenu
                        // Call the best scores submenu function here
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        // Check if any main button was clicked
                        for (int i = 0; i < 3; i++) {
                            if (is_mouse_over_button(&buttons[i], event.button.x, event.button.y)) {
                                if (i == 2) { // Retour button
                                    quitter = 0;
                                    *menustate = MENU_MAIN;
                                    printf("Retour button clicked!\n");
                                } else {
                                    showSubButtons = 1; // Show sub buttons
                                    buttons[0].isActive = 0; // Hide main buttons
                                    buttons[1].isActive = 0;
                                    buttons[2].isActive = 0;
                                    printf("Main button clicked!\n");
                                }
                            }
                        }
                        // Check if any sub button was clicked
                        if (showSubButtons) {
                            for (int i = 3; i < 9; i++) {
                                if (is_mouse_over_button(&buttons[i], event.button.x, event.button.y)) {
                                    printf("Sub button clicked!\n");
                                    // Handle sub button click
                                    if (i == 8) { // Retour button
                                        showSubButtons = 0; // Hide sub buttons
                                        buttons[0].isActive = 1; // Show main buttons
                                        buttons[1].isActive = 1;
                                        buttons[2].isActive = 1;
                                        printf("Retour button clicked!\n");
                                    }
                                }
                            }
                        }
                    }
                    break;

                case SDL_MOUSEMOTION:
                    // Update button states for all buttons
                    for (int i = 0; i < 9; i++) {
                        update_button_state(&buttons[i], event.motion.x, event.motion.y);
                    }
                    break;
            }
        }

        // Render main buttons if not hidden
        if (!showSubButtons) {
            for (int i = 0; i < 3; i++) {
                render_button(ecran, &buttons[i]);
            }
        }

        // Render sub buttons if the flag is set
        if (showSubButtons) {
            for (int i = 3; i < 9; i++) {
                render_button(ecran, &buttons[i]);
            }
        }

        SDL_Flip(ecran); // Update screen
    }

    // Cleanup resources
    for (int i = 0; i < 9; i++) {
        if (buttons[i].normalImage) SDL_FreeSurface(buttons[i].normalImage);
        if (buttons[i].hoverImage) SDL_FreeSurface(buttons[i].hoverImage);
    }
}