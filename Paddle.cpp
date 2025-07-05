
#include "Paddle.h"

Paddle::Paddle(bool player)
{
    

    //Initialize the offsets
    if(player){
        mPosX = field_rect.x + 2*BAR_WIDTH;
    }
    else
    {
        mPosX = field_rect.x + field_rect.w - 3*BAR_WIDTH;
    }
    
    mPosY = SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    collisionRect.h = PADDLE_HEIGHT;
    collisionRect.w = BAR_WIDTH;
    collisionRect.x = mPosX;
    collisionRect.y = mPosY;
    
    blue = player;
    player1 = player; 

}

void Paddle::handleEvent( SDL_Event& e )
{
    if(!player1)
    {
        //If a key was pressed
        if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
        {
            //Adjust the velocity
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: mVelY -= PADDLE_VEL; break;
                case SDLK_DOWN: mVelY += PADDLE_VEL; break;
                case SDLK_LEFT: mVelX -= PADDLE_VEL; break;
                case SDLK_RIGHT: mVelX += PADDLE_VEL; break;
            }
        }
        //If a key was released
        else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
        {
            //Adjust the velocity
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: mVelY += PADDLE_VEL; break;
                case SDLK_DOWN: mVelY -= PADDLE_VEL; break;
                case SDLK_LEFT: mVelX += PADDLE_VEL; break;
                case SDLK_RIGHT: mVelX -= PADDLE_VEL; break;
            }
        }
    }
    else
    {
        //If a key was pressed
        if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
        {
            //Adjust the velocity
            switch( e.key.keysym.sym )
            {
                case SDLK_w: mVelY -= PADDLE_VEL; break;
                case SDLK_s: mVelY += PADDLE_VEL; break;
                case SDLK_a: mVelX -= PADDLE_VEL; break;
                case SDLK_d: mVelX += PADDLE_VEL; break;
            }
        }
        //If a key was released
        else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
        {
            //Adjust the velocity
            switch( e.key.keysym.sym )
            {
                case SDLK_w: mVelY += PADDLE_VEL; break;
                case SDLK_s: mVelY -= PADDLE_VEL; break;
                case SDLK_a: mVelX += PADDLE_VEL; break;
                case SDLK_d: mVelX -= PADDLE_VEL; break;
            }
        }
    }
    
}

void Paddle::move()
{
    //Move the dot left or right
    mPosX += mVelX;
    collisionRect.x = mPosX;

    //If the dot went too far to the left or right
    if(player1){
        if( ( mPosX < field_rect.x + 2*BAR_WIDTH  ) || ( mPosX + 2*BAR_WIDTH > SCREEN_WIDTH/2) )
        {
            //Move back
            mPosX -= mVelX;
            collisionRect.x = mPosX;
        }
    }
    else
    {
        if(( mPosX - BAR_WIDTH < SCREEN_WIDTH/2) || ( mPosX > field_rect.x + field_rect.w - 3*BAR_WIDTH  ) )
        {
            //Move back
            mPosX -= mVelX;
            collisionRect.x = mPosX;
        }
    }
    



    //Move the dot up or down
    mPosY += mVelY;
    collisionRect.y = mPosY;

    //If the dot went too far up or down
    if( ( mPosY < field_rect.y + BAR_WIDTH + 1) || ( mPosY + PADDLE_HEIGHT > field_rect.y + FIELD_HEIGHT - BAR_WIDTH - 1) )
    {
        //Move back
        mPosY -= mVelY;
        collisionRect.y = mPosY;
    }
}

void Paddle::render()
{
    // Set color of ball
    blue ? SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF) : SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF);   
    SDL_Rect paddle_rect = {mPosX, mPosY, BAR_WIDTH, PADDLE_HEIGHT};
    SDL_RenderFillRect( renderer, &paddle_rect );
    
}
