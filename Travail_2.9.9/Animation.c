#include "Animation.h"
#include "../Gestion_Erreurs.c"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* birdTexture = NULL;
SDL_Texture* pipeTexture = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* gameOverTextTexture = NULL;
SDL_Texture* cloudTexture = NULL;
TTF_Font* gameOverFont = NULL;
SDL_Color gameOverColor = {0, 0, 0, 255};

SDL_Rect birdRect = { 100, WINDOW_HEIGHT / 2 - BIRD_SIZE / 2, BIRD_SIZE, BIRD_SIZE };
int birdVelocity = 0;

Pipe pipe1, pipe2;
int score = 0;
bool game_over = false;
int pipeSpaceStart = 0;

void generatePipes()
{
    pipe1.spaceStart = rand() % (WINDOW_HEIGHT - PIPE_GAP);
    pipe2.spaceStart = pipe1.spaceStart;
    pipeSpaceStart = pipe1.spaceStart;
}

void initializeGame()
{
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == NULL) {
        erreur("Unable to create window");
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        erreur("Unable to create renderer");
    }

    SDL_Surface* birdSurface = IMG_Load("Images/bird.png");
    if (birdSurface == NULL)
    {
        erreur("Unable to load image bird.png");
    }
    birdTexture = SDL_CreateTextureFromSurface(renderer, birdSurface);
    SDL_FreeSurface(birdSurface);

    SDL_Surface* pipeSurface = IMG_Load("Images/pipe.png");
    if (pipeSurface == NULL)
    {
        erreur("Unable to load image pipe.png");
    }
    pipeTexture = SDL_CreateTextureFromSurface(renderer, pipeSurface);
    SDL_FreeSurface(pipeSurface);

    SDL_Surface* cloudSurface = IMG_Load("Images/clouds.jpg");
    if (cloudSurface == NULL)
    {
        erreur("Unable to load image clouds.jpg");
    }
    cloudTexture = SDL_CreateTextureFromSurface(renderer, cloudSurface);
    SDL_FreeSurface(cloudSurface);

    SDL_Surface* backgroundSurface = IMG_Load("Images/background.jpg");
    if (backgroundSurface == NULL)
    {
        erreur("Unable to load image background.jpg");
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    gameOverFont = TTF_OpenFont("Font/arial_bold.ttf", 42);
    if (gameOverFont == NULL)
    {
        erreur("Unable to load font arial_bold.ttf");
    }

    pipe1.x = WINDOW_WIDTH;
    pipe2.x = WINDOW_WIDTH + WINDOW_WIDTH / 2;
    generatePipes();

    // Reset game state
    game_over = false;
    score = 0;
    birdVelocity = 0;

    birdRect.y = WINDOW_HEIGHT / 2 - BIRD_SIZE / 2;  // Reset bird position
}

void closeGame()
{
    SDL_DestroyTexture(birdTexture);
    SDL_DestroyTexture(pipeTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gameOverTextTexture);
    SDL_DestroyTexture(cloudTexture);
    TTF_CloseFont(gameOverFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void updateGame()
{
    if (game_over)
    {
        return;
    }

    birdVelocity += GRAVITY;
    birdRect.y += birdVelocity;

    pipe1.x -= 2;
    if (pipe1.x + PIPE_WIDTH < birdRect.x)
    {
        pipe1.x = pipe2.x + WINDOW_WIDTH / 2;
        generatePipes();
        score++;
    }

    pipe2.x -= 2;
    if (pipe2.x + PIPE_WIDTH < birdRect.x)
    {
        pipe2.x = pipe1.x + WINDOW_WIDTH / 2;
        generatePipes();
        score++;
    }

    // Collision detection
    if (birdRect.y <= 0 || birdRect.y + birdRect.h >= WINDOW_HEIGHT)
    {
        game_over = true;
        printf("Game Over! Your Score: %d\n", score);
    }

    if ((birdRect.x + birdRect.w > pipe1.x && birdRect.x < pipe1.x + PIPE_WIDTH && (birdRect.y < pipe1.spaceStart || birdRect.y + birdRect.h > pipe1.spaceStart + PIPE_GAP)) ||
        (birdRect.x + birdRect.w > pipe2.x && birdRect.x < pipe2.x + PIPE_WIDTH && (birdRect.y < pipe2.spaceStart || birdRect.y + birdRect.h > pipe2.spaceStart + PIPE_GAP))) {
        game_over = true;
        printf("Game Over! Your Score: %d\n", score);
    }

    if (game_over)
    {
        char game_over_text[50];
        sprintf(game_over_text, "Game Over! Press Y to Play Again or N to Quit.");
        SDL_Surface* gameOverSurface = TTF_RenderText_Solid(gameOverFont, game_over_text, gameOverColor);
        if (gameOverSurface == NULL)
        {
            avertissement("Failed to render game over text");
        }
        else
        {
            gameOverTextTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
            SDL_FreeSurface(gameOverSurface);
        }
    }
}

void drawGame()
{
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Rect cloudRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(renderer, cloudTexture, NULL, &cloudRect);

    SDL_Rect pipeRect = { pipe1.x, 0, PIPE_WIDTH, pipe1.spaceStart };
    SDL_RenderCopy(renderer, pipeTexture, NULL, &pipeRect);

    pipeRect.y = pipe1.spaceStart + PIPE_GAP;
    pipeRect.h = WINDOW_HEIGHT - pipeRect.y;
    SDL_RenderCopy(renderer, pipeTexture, NULL, &pipeRect);

    pipeRect.x = pipe2.x;
    pipeRect.y = 0;
    pipeRect.h = pipe2.spaceStart;
    SDL_RenderCopy(renderer, pipeTexture, NULL, &pipeRect);

    pipeRect.y = pipe2.spaceStart + PIPE_GAP;
    pipeRect.h = WINDOW_HEIGHT - pipeRect.y;
    SDL_RenderCopy(renderer, pipeTexture, NULL, &pipeRect);

    SDL_RenderCopy(renderer, birdTexture, NULL, &birdRect);

    if (game_over)
    {
        SDL_Rect textRect = { 50, WINDOW_HEIGHT / 2 - 50, 540, 100 };
        SDL_RenderCopy(renderer, gameOverTextTexture, NULL, &textRect);
    }

    SDL_RenderPresent(renderer);
}

void runGame()
{
    initializeGame();

    SDL_Event event;

    while (1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                closeGame();
                return;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (game_over)
                {
                    if (event.key.keysym.sym == SDLK_y)
                    {
                        closeGame(); // Close the current game state
                        initializeGame(); // Initialize a new game state
                    }
                    else if (event.key.keysym.sym == SDLK_n)
                    {
                        closeGame();
                        return;
                    }
                }
                else
                {
                    if (event.key.keysym.sym == SDLK_SPACE)
                    {
                        birdVelocity = -JUMP_FORCE;
                    }
                }
            }
        }

        updateGame();
        drawGame();
        SDL_Delay(16);
    }
}

int main()
{
    runGame();
    return 0;
}
