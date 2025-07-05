#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>



//Screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int FIELD_WIDTH;
extern const int FIELD_HEIGHT;
extern const int BAR_WIDTH;
extern int PADDLE_HEIGHT;
extern SDL_Renderer* renderer;
extern SDL_Window* window;
extern SDL_Rect field_rect;

extern const int NO_COLLISION;
extern const int PLAYER1_COLLISION;
extern const int PLAYER2_COLLISION;
extern const int LEFT_GOAL_COLLISION;
extern const int RIGHT_GOAL_COLLISION;
extern const int POWERUP_COLLISION;
//Globally used font
extern TTF_Font* font;


