#include "pong.h"

class Paddle
{
    public:
        //The dimensions of the paddle

        //Maximum axis velocity of the paddle
        static const int PADDLE_VEL = 6;

        //Paddle's collision box
        SDL_Rect collisionRect;

        
        bool player1;

        //The X and Y offsets of the paddle
        int mPosX;
        int mPosY;

        int mHeight;

        //The velocity of the paddle
        int mVelX, mVelY;


        bool blue;

        // Powerups for paddle as boolean
        bool powerups[2];

        //Initializes the variables
        Paddle(bool player1);

        //Takes key presses and adjusts the paddle's velocity
        void handleEvent( SDL_Event& e );

        //Moves the paddle
        void move();

        //Shows the paddle on the screen
        void render();

    private:
        


        
        
};