/* MINES HEADDER */

#ifndef MINE_LOGIC
#define MINE_LOGIC

#include <time.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <math.h>
#include <stdio.h>
#include <SDL3_image/SDL_image.h>

struct mine{
    int state;
    bool highlight;
    bool hidden;
    bool flagged;
};

void pass(double m);

void updateSize(SDL_Renderer*render);

void floodFill(int x, int y, struct mine mineStates[128*128]);

void tick(struct mine mineStates[128*128]);

void render9ScaleRotate(SDL_Renderer *render,SDL_Texture *Spritesheet,SDL_FRect *position, int deg,SDL_FRect gridTile );

void renderBorders(SDL_Renderer *render,SDL_Texture* Spritesheet,SDL_FRect gridTile );

void renderClock(SDL_Renderer * render, SDL_Texture* Spritesheet, int num,int x,int y);

void renderGrid(SDL_Renderer *render,SDL_Texture* spritesheet, int mines[128][128],struct mine minestates[128*128]  );
#endif
