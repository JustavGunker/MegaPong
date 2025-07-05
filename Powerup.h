#include "pong.h"

class Powerup
{
    public:

        //Initializes the variables
        Powerup(int x, int y, int type);

        int power;

        //The X and Y offsets of the powerup
        int mPosX;
        int mPosY;


        //Ball's collision box
        SDL_Rect mCollider;

        bool checkCollision( SDL_Rect a);
};

