
#include "menuEnigme.h"
#include "../header.h"

static int showSubButtons = 0;
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
    menus[MENU_ENIGME].buttons = malloc(menus[MENU_ENIGME].n_btns * sizeof(Button));
    if (!menus[MENU_ENIGME].buttons) {
        fprintf(stderr, "Failed to allocate memory for buttons\n");
        exit(EXIT_FAILURE);
    }

    menus[MENU_ENIGME].init_buttons = initMenuEnigmeButtons;
    menus[MENU_ENIGME].render = renderMenuEnigme;
    menus[MENU_ENIGME].handleEvent = handleEventEnigme;
    menus[MENU_ENIGME].init_buttons(menus[MENU_ENIGME].buttons);

    srand(time(NULL)); // Seed the random number generator
    loadQuestions("./menu/menu_enigme/questions.txt", questions, &numQuestions);
    if (numQuestions == 0) {
        fprintf(stderr, "No questions loaded. Exiting.\n");
        exit(EXIT_FAILURE);
    }
}

void initMenuEnigmeButtons(Button *buttons) {
    buttons[0].normalImage = load_image("./assets/buttons/quiz.png");
    buttons[0].hoverImage = load_image("./assets/buttons/quizhov.png");
    buttons[0].rect = (SDL_Rect){600, 200, 300, 40};
    buttons[0].selected = 0;

    buttons[1].normalImage = load_image("./assets/buttons/puzzle.png");
    buttons[1].hoverImage = load_image("./assets/buttons/puzzlehov.png");
    buttons[1].rect = (SDL_Rect){1000, 200, 300, 40};
    buttons[1].selected = 0;

    for (int i = 2; i < 5; i++) {
        buttons[i].normalImage = load_image("./assets/buttons/butbase.png");
        buttons[i].hoverImage = load_image("./assets/buttons/butbase.png");
        buttons[i].rect = (SDL_Rect){400, 300 + (i - 2) * 200, 300, 40};
        buttons[i].selected = 0;
    }
}

void renderMenuEnigme(SDL_Surface *background, SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    SDL_BlitSurface(background, NULL, screen, NULL);

    char scoreText[50], livesText[50];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
    snprintf(livesText, sizeof(livesText), "Lives: %d", lives);

    renderText(screen, scoreText, font, textColor, 50, 50);
    renderText(screen, livesText, font, textColor, 50, 100);

    if (!showSubButtons) {
        for (int i = 0; i < 2; i++) {
            renderButton(screen, font, textColor, buttons[i]);
        }
    } else {
        displayQuizUI(screen, font, textColor, buttons, n_btns);
    }

    SDL_Flip(screen);
}

void displayQuizUI(SDL_Surface *screen, TTF_Font *font, SDL_Color textColor, Button *buttons, int n_btns) {
    if (currentQuestionIndex == -1) {
        currentQuestionIndex = 0;
    }

    if (currentQuestionIndex >= numQuestions) {
        printf("All questions answered. Resetting...\n");
        currentQuestionIndex = 0;
    }

    Question currentQuestion = questions[currentQuestionIndex];

    renderText(screen, "Quiz", font, textColor, 600, 100);
    renderText(screen, currentQuestion.question, font, textColor, 400, 200);

    for (int i = 2; i < n_btns; i++) {
        renderButton(screen, font, textColor, buttons[i]);
    }

    renderText(screen, currentQuestion.options[0], font, textColor, 425, 300);
    renderText(screen, currentQuestion.options[1], font, textColor, 425, 500);
    renderText(screen, currentQuestion.options[2], font, textColor, 425, 700);

    SDL_Flip(screen);
}

void loadQuestions(const char *filename, Question *questions, int *numQuestions) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open questions file: %s\n", filename);
        return;
    }

    char line[256];
    int questionIndex = 0;

    while (fgets(line, sizeof(line), file) && questionIndex < MAX_QUESTIONS) {
        if (sscanf(line, "%255[^;];%255[^;];%255[^;];%255[^;];%d",
                   questions[questionIndex].question,
                   questions[questionIndex].options[0],
                   questions[questionIndex].options[1],
                   questions[questionIndex].options[2],
                   &questions[questionIndex].correctOption) == 5) {
            questionIndex++;
        } else {
            fprintf(stderr, "Failed to parse line: %s\n", line);
        }
    }

    *numQuestions = questionIndex;
    fclose(file);

    for (int i = 0; i < *numQuestions; i++) {
        int j = rand() % *numQuestions;
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

void checkAnswer(int selectedOption, int correctOption, int *score, int *lives, int *level, int *menuState) {
    if(selectedOption >  -1){
        if (selectedOption == correctOption) {
            printf("Correct answer!\n");
            (*score)++;
            if (*score % 5 == 0) {
                (*level)++;
            }
        } else {
            printf("Wrong answer!\n");
            if (*lives > 0) {
                (*lives)--;
            }
            if (*lives == 0) {
                printf("Returning to Menu Option...\n");
                *score = 0;
                *lives = 3;
                *level = 1;
                *menuState = MENU_OPTION;
            }
        }
        if (*score == 10){
            *menuState = MENU_PRINCIPAL;
        }
        currentQuestionIndex++;
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
                    buttons[i].selected = 1;
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
                            showSubButtons = 1;
                        } else if (i == 1) {
                            printf("Puzzle selected\n");
                        }
                        if (showSubButtons) {
                            checkAnswer(i -2 , questions[currentQuestionIndex].correctOption, &score, &lives, &level, menuState);
                        }
                    }
                }
            }
        }
    }
}