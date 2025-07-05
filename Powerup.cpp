#include "Powerup.h"

Powerup::Powerup(int x, int y, int type)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;


    //Set collision box dimension
    mCollider.w = 23;
    mCollider.h = 23;
    mCollider.x = mPosX - 11;
    mCollider.y = mPosY - 11;

    power = type;
}

bool Powerup::checkCollision( SDL_Rect a )
{

    SDL_Rect b = mCollider;
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}
