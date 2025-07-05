//#include "pong.h"
#include "Ball.h"

Ball::Ball(int x, int y, bool player1scored)
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

    //Initialize the velocity
    mVelX = player1scored ? 5 : -5;
    mVelY = 0; //(rand() % 5) - (rand() % 5);

    //Set collision box dimension
    mCollider.w = 2*BALL_RADIUS;
    mCollider.h = 2*BALL_RADIUS;

    blue = player1scored;
}


int Ball::clipVel(int vel)
{

    if (vel < 0)
    {
        if (vel < -1*BALL_VEL_MAX)
        {
            return -1*BALL_VEL_MAX;
        }
        if (vel > -1*BALL_VEL_MIN)
        {
            return -1*BALL_VEL_MIN;
        }
    }

    if (vel > 0)
    {
        if (vel > BALL_VEL_MAX)
        {
            return BALL_VEL_MAX;
        }
        if (vel < BALL_VEL_MIN)
        {
            return BALL_VEL_MIN;
        }
    }
    return vel;
}

void Ball::paddleReact(Paddle paddle, int* currentCollision, int lastCollision)
{

    if (checkCollision(mCollider,paddle.collisionRect))
    {
        
        if(lastCollision != PLAYER1_COLLISION && paddle.player1)
        {
            mVelX *= -1;
        }
        else if (lastCollision != PLAYER2_COLLISION && !paddle.player1)
        {
            mVelX *= -1;
        }

        // Add paddle speed unless it is stuck to the edges
        if (!(paddle.collisionRect.y <= field_rect.y + BAR_WIDTH + 1 + 5) && !( paddle.collisionRect.y + paddle.collisionRect.h >= field_rect.y + FIELD_HEIGHT - BAR_WIDTH - 1 - 5)) 
        {
            mVelY += paddle.mVelY;
        }
        
        if(paddle.player1){
            if (!(paddle.collisionRect.x <= field_rect.x + 2*BAR_WIDTH + 10) && !(paddle.collisionRect.x + 2*BAR_WIDTH >= SCREEN_WIDTH/2 ))
            {
                mVelX += paddle.mVelX;
            }
        } else
        {
            if (!(paddle.collisionRect.x >= field_rect.x + field_rect.w - 3*BAR_WIDTH - 10) && !(paddle.collisionRect.x <= SCREEN_WIDTH/2 + BAR_WIDTH))
            {
                mVelX += paddle.mVelX;
            }
        }
        
        
        mCollider.x = mPosX - BALL_RADIUS;
        blue = paddle.blue;
        *currentCollision = paddle.player1 ?  PLAYER1_COLLISION : PLAYER2_COLLISION;
    }

}


int Ball::move(Paddle paddle1, Paddle paddle2, int lastCollision)
{
    int collision = NO_COLLISION;

    paddleReact(paddle1, &collision, lastCollision);
    paddleReact(paddle2, &collision, lastCollision);


    mVelX = clipVel(mVelX);
    mVelY = clipVel(mVelY);
    //Move the dot left or right
    mPosX += mVelX;
    mCollider.x = mPosX - BALL_RADIUS;
    //If the dot went too far to the left or right
    if( ( mPosX - BALL_RADIUS < field_rect.x ) )
    {
        //Move back
        mPosX -= mVelX;
       
        mCollider.x = mPosX - BALL_RADIUS;
        blue = false;
        collision = LEFT_GOAL_COLLISION;
    }
    if(( mPosX + BALL_RADIUS > field_rect.x + FIELD_WIDTH - 1) )
    {
        //Move back
        mPosX -= mVelX;

        mCollider.x = mPosX - BALL_RADIUS;
        blue = false;
        collision = RIGHT_GOAL_COLLISION;
    }
    
    


    //Move the dot up or down
    mPosY += mVelY;
    mCollider.y = mPosY - BALL_RADIUS;

    //If the dot went too far up or down
    if( ( mPosY - BALL_RADIUS < field_rect.y + BAR_WIDTH) || ( mPosY + BALL_RADIUS > field_rect.y + FIELD_HEIGHT - BAR_WIDTH - 1))
    {
        //Move back
        mPosY -= mVelY;
        mVelY *= -1;
        mCollider.y = mPosY - BALL_RADIUS;

    }

    return collision;
}



void Ball::render()
{
    // Set color of ball
    blue ? SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF) : SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF);   

    const int32_t diameter = (BALL_RADIUS * 2);
    int centreX = mPosX;
    int centreY = mPosY;
    int32_t x = (BALL_RADIUS - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

bool Ball::checkCollision( SDL_Rect a, SDL_Rect b )
{
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