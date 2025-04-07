#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "menuEnigme.h"
#include "../header.h"

static int showSubButtons = 0;
static int askedQuestions[MAX_QUESTIONS] = {0};
static int currentQuestionIndex = -1;
static int score = 0;
static int lives = 3;
static int level = 1;
static Question questions[MAX_QUESTIONS];
static int numQuestions = 0;

Mix_Music *suspenseMusic = NULL;

void initMenuEnigme(Menu *menus) {
    printf("Init Menu Enigme\n");
    menus[MENU_ENIGME].n_btns = 5;
    menus[MENU_ENIGME].buttons = malloc(menus[MENU_ENIGME].n_btns * sizeof(Button)); // Allocate memory for buttons
    menus[MENU_ENIGME].init_buttons = initMenuEnigmeButtons;
    menus[MENU_ENIGME].render = renderMenuEnigme;
    menus[MENU_ENIGME].handleEvent = handleEventEnigme;
    menus[MENU_ENIGME].init_buttons(menus[MENU_ENIGME].buttons);
    loadQuestions("questions.txt", questions, &numQuestions);
}

void initMenuEnigmeButtons(Button *buttons) {
    buttons[0].normalImage = load_image("./assets/buttons/quiz.png");
    buttons[0].hoverImage = load_image("./assets/buttons/quizhov.png");
    buttons[0].rect = (SDL_Rect){600,200,300,40}; // Quiz
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("./assets/buttons/puzzle.png");
    buttons[1].hoverImage = load_image("./assets/buttons/puzzlehov.png");
    buttons[1].rect = (SDL_Rect){1000,200,300,40}; // Puzzle
    buttons[1].selected = 0;

    buttons[2].normalImage = load_image("./assets/buttons/butbase.png");
    buttons[2].hoverImage = load_image("./assets/buttons/butbase.png");
    buttons[2].rect = (SDL_Rect){400,300,300,40}; // Option 1
    buttons[2].selected = 0;

    buttons[3].normalImage = load_image("./assets/buttons/butbase.png");
    buttons[3].hoverImage = load_image("./assets/buttons/butbase.png");
    buttons[3].rect = (SDL_Rect){400,400,300,40}; // Option 2
    buttons[3].selected = 0;

    buttons[4].normalImage = load_image("./assets/buttons/butbase.png");
    buttons[4].hoverImage = load_image("./assets/buttons/butbase.png");
    buttons[4].rect = (SDL_Rect){400,500,300,40}; // Option 3
    buttons[4].selected = 0;
}

void renderMenuEnigme(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);
    if (!showSubButtons) {
        for (int i = 0; i < 2; i++) {
            renderButton(screen, font, textColor, buttons[i]);
        }
    } else {
        displayQuizUI(screen, TTF_OpenFont("./assets/fonts/font.ttf", 40), (SDL_Color){255, 255, 255, 0},buttons,n_btns);
    }

    SDL_Flip(screen);
}

void displayQuizUI(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    if (currentQuestionIndex == -1) {
        currentQuestionIndex = getRandomQuestionIndex(askedQuestions, numQuestions);
    }

    Question currentQuestion = questions[currentQuestionIndex];

    // Display the "Quiz" message
    renderText(screen, "Quiz", font, textColor, 600, 100);

    // Display the question
    renderText(screen, currentQuestion.question, font, textColor, 400, 200);

    // Render the buttons
    for (int i = 2; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]);
    }

    // Display text below the buttons
    renderText(screen, currentQuestion.options[0], font, textColor, 425, 300);
    renderText(screen, currentQuestion.options[1], font, textColor, 425, 400);
    renderText(screen, currentQuestion.options[2], font, textColor, 425, 500);

    SDL_Flip(screen);
}

void loadQuestions(const char *filename, Question *questions, int *numQuestions) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open questions file: %s\n", filename);
        return;
    }

    char line[256];
    int questionIndex = 0;
    printf("aaaaaaaaaaaaaa");
    while (fgets(line, sizeof(line), file) && questionIndex < MAX_QUESTIONS) {
        // Utilisation de sscanf avec des limites de taille pour éviter les débordements de tampon
        if (sscanf(line, "%255[^;];%255[^;];%255[^;];%255[^;];%d",
                   questions[questionIndex].question,
                   questions[questionIndex].options[0],
                   questions[questionIndex].options[1],
                   questions[questionIndex].options[2],
                   &questions[questionIndex].correctOption) == 5) {
            questionIndex++;
        } else {
            printf("Failed to parse line: %s\n", line);
        }
    }

    *numQuestions = questionIndex;
    fclose(file);
}

int getRandomQuestionIndex(int *askedQuestions, int numQuestions) {
    int index;
    do {
        index = rand() % numQuestions;
    } while (askedQuestions[index]);

    askedQuestions[index] = 1;
    return index;
}

void checkAnswer(int selectedOption, int correctOption, int *score, int *lives, int *level) {
    if (selectedOption == correctOption) {
        printf("Correct answer!\n");
        (*score)++;
        if (*score % 5 == 0) {
            (*level)++;
        }
    } else {
        printf("Wrong answer!\n");
        (*lives)--;
        if (*lives == 0) {
            printf("Game Over!\n");
            // Handle game over logic
        }
    }
}

void handleEventEnigme(int *menuState, SDL_Event event, Button *buttons, int n_btns, Mix_Chunk *hoverSound) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            *menuState = QUIT_GAME;
        }
        if (event.type == SDL_MOUSEMOTION) {
            for (int i = 0; i < n_btns; i++) {
                if (event.motion.x >= buttons[i].rect.x && event.motion.x <= buttons[i].rect.x + buttons[i].rect.w &&
                    event.motion.y >= buttons[i].rect.y && event.motion.y <= buttons[i].rect.y + buttons[i].rect.h) {
                    if (!buttons[i].selected) {
                        buttons[i].selected = 1;
                        //Mix_PlayChannel(-1, hoverSound, 0);
                    }
                } else {
                    buttons[i].selected = 0;
                }
            }
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < n_btns; i++) {
                    if (buttons[i].selected) {
                        if (i == 0) {
                            printf("Quiz selected\n");
                            showSubButtons = 1; // Show sub buttons
                            buttons[0].selected = 0; // Hide main buttons
                            buttons[1].selected = 0;
                            // Play suspense music
                            if (suspenseMusic == NULL) {
                                suspenseMusic = Mix_LoadMUS("./assets/music/suspense.mp3");
                                if (!suspenseMusic) {
                                    printf("Failed to load suspense music: %s\n", Mix_GetError());
                                }
                            }
                            Mix_PlayMusic(suspenseMusic, 1);
                            
                        } else if (i == 1) {
                            printf("Puzzle selected\n");
                            // Display Puzzle UI
                        }
                        if (showSubButtons) {
                            checkAnswer(i - 2, questions[currentQuestionIndex].correctOption, &score, &lives, &level);
                            currentQuestionIndex = getRandomQuestionIndex(askedQuestions, numQuestions);
                        }
                    }
                }
            }
        }
    }
}
