#include "pong.h"
#include "Paddle.h"
#include "Powerup.h"
#include <vector>
#include <bits/stdc++.h>

class Ball
{
    public:
        //The dimensions of the dot
        static const int BALL_RADIUS = 8;
        static const int BALL_VEL_MIN = 4;
        static const int BALL_VEL_MAX = 16;

        //Maximum axis velocity of the dot
        static const int BALL_VEL = 8;

        bool blue;

        //Initializes the variables
        Ball(int x, int y, bool player1scored);


        int clipVel(int vel);

        void paddleReact(Paddle paddle, int* currentCollision, int lastCollision);

        //Moves the dot
        int move(Paddle paddle1, Paddle paddle2, int lastCollision);



        //Shows the dot on the screen
        void render();

        //Box collision detector
        bool checkCollision( SDL_Rect a, SDL_Rect b );

        //Ball's collision box
        SDL_Rect mCollider;

    private:
        //The X and Y offsets of the dot
        int mPosX;
        int mPosY;

        //The velocity of the dot
        int mVelX, mVelY;

        
        
};