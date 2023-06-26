#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define PIPE_WIDTH 80
#define PIPE_GAP 150
#define BIRD_SIZE 32
#define GRAVITY 1
#define JUMP_FORCE 10

typedef struct {
    int x;
    int spaceStart;
} Pipe;

void generatePipes();
void initializeGame();
void closeGame();
void updateGame();
void drawGame();
void runGame();

#endif /* ANIMATION_H */
