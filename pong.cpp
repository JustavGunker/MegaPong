#include "Ball.h"
#include "Texture.h"



//Screen dimension constants
const int SCREEN_WIDTH = 840;
const int SCREEN_HEIGHT = 480;
const int FIELD_WIDTH = 640;
const int FIELD_HEIGHT = 280;
const int BAR_WIDTH = 8;
int PADDLE_HEIGHT = 70;
SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
TTF_Font* font = NULL;
LTexture textTexture;
LTexture gTimeTextTexture;
SDL_Rect spriteClips[ 10 ];
SDL_Rect powerupClips[ 3 ];
LTexture playerScoreTexture;
LTexture powerupTexture;
SDL_Rect field_rect = {(SCREEN_WIDTH - FIELD_WIDTH)/2, (SCREEN_HEIGHT - FIELD_HEIGHT)/2, FIELD_WIDTH, FIELD_HEIGHT};


const int NO_COLLISION = 0;
const int PLAYER1_COLLISION = 1;
const int PLAYER2_COLLISION = 2;
const int LEFT_GOAL_COLLISION = 3;
const int RIGHT_GOAL_COLLISION = 4;
const int POWERUP_COLLISION = 5;

bool initWindow(SDL_Window** p_sdl_window,SDL_Surface** p_screen_surface, SDL_Renderer **p_renderer)
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        *p_sdl_window = SDL_CreateWindow( "MegaPong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( *p_sdl_window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else 
        {
            //Create renderer for window
            *p_renderer = SDL_CreateRenderer( *p_sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( *p_renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( *p_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
            }
            //Initialize SDL_ttf
            if( TTF_Init() == -1 )
            {
                printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                success = false;
            }


            //Get window surface
            *p_screen_surface = SDL_GetWindowSurface( *p_sdl_window );

            
        }

    }
    return success;
}

bool loadMedia(SDL_Surface** p_media_surface)
{
    //Loading success flag
    bool success = true;

    //Load splash image
    *p_media_surface = SDL_LoadBMP( "BG1.bmp" );
    if( *p_media_surface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "BG1.bmp", SDL_GetError() );
        success = false;
    }

    

    return success;
}

bool loadNumbers()
{
    //Loading success flag
    bool success = true;

    //Load sprite sheet texture
    if( !playerScoreTexture.loadFromFile( "numbers.png" ))
    {
        printf( "Failed to load numbers texture!\n" );
        success = false;
    }
    else
    {   
        for(int i = 0; i < 5; i++) // First row in img
        {
            //Set sprite clips
            spriteClips[ i ].x =   i*60;
            spriteClips[ i ].y =   0;
            spriteClips[ i ].w =  60;
            spriteClips[ i ].h = 80;
        }
        for(int i = 5; i < 10; i++) // second row in img
        {
            //Set sprite clips
            spriteClips[ i ].x =   (i-5)*60;
            spriteClips[ i ].y =   80;
            spriteClips[ i ].w =  60;
            spriteClips[ i ].h = 80;
        }
    }
    
    return success;
}

bool loadPowerups()
{
    //Loading success flag
    bool success = true;

    //Load sprite sheet texture
    if( !powerupTexture.loadFromFile( "powerups.png" ))
    {
        printf( "Failed to load powerup texture!\n" );
        success = false;
    }
    else
    {   
        for(int i = 0; i < 3; i++) 
        {
            //Set sprite clips
            powerupClips[ i ].x = i*23;
            powerupClips[ i ].y = 0;
            powerupClips[ i ].w = 23;
            powerupClips[ i ].h = 22;
        }
    }
    
    return success;
}


bool loadTextMedia(LTexture* texture, std::string string)
{
    //Loading success flag
    bool success = true;

    //Open the font
    font = TTF_OpenFont( "lazy.ttf", 28 );
    if( font == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
    SDL_Color textColor = { 0, 0, 0 };
    if( !textTexture.loadFromRenderedText( string, textColor ) )
    {
        printf( "Failed to render text texture!\n" );
        success = false;
    }

    return success;
    }

    return success;
}

Ball initBall(int x, int y, int velX, int velY, bool player1goal)
{
    Ball ball(x,y,velX,velY,player1goal);
    return ball;
}



Powerup initPowerup(int x, int y, int type)
{
  
    Powerup powerup(x, y, type);

    return powerup;
}




void drawField(SDL_Renderer *renderer, SDL_Rect *field)
{
    int startX = (*field).x + (*field).w/2;
    int startY = (*field).y + 1;
    int endY = startY + (*field).h - 2;


    SDL_Rect topBar = {(*field).x+1, startY, (*field).w-2, BAR_WIDTH};
    SDL_Rect botBar = {(*field).x+1, endY - BAR_WIDTH , (*field).w-2, BAR_WIDTH};
    //Render field
    SDL_SetRenderDrawColor( renderer, 0, 200,50,50 );        
    SDL_RenderFillRect( renderer, field );

    //Render black outlined quad
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF );        
    SDL_RenderDrawRect( renderer, field );

    //Draw vertical line 
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderDrawLine(renderer,startX, endY - 1, startX, startY); 
    SDL_RenderFillRect( renderer, &topBar );
    SDL_RenderFillRect( renderer, &botBar );

}


void closeWindow(SDL_Surface** p_media_surface)
{
    //Free loaded images
    textTexture.free();
    playerScoreTexture.free();
    powerupTexture.free();

    //Free global font
    TTF_CloseFont( font );
    font = NULL;

    //Deallocate surfaces
    SDL_FreeSurface( *p_media_surface );
    *p_media_surface = NULL;

    //Destroy window
    SDL_DestroyWindow( window );
    SDL_DestroyRenderer( renderer );
    window = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void DeleteBalls(std::vector<Ball>* vec, std::vector<int>* staleBalls)
{
    for(int i = 0; i < staleBalls->size(); i++)
    {
        int index = staleBalls->at(i);
        if (index >= 0 && index < vec->size()) {
            vec->erase(vec->begin() + index);
        }
        
    }
    staleBalls->clear();
}

void DeletePowerups(std::vector<Powerup>* vec, std::vector<int>* stalePU)
{

    for(int i = 0; i < stalePU->size(); i++)
    {
        int index = stalePU->at(i);
        if (index >= 0 && index < vec->size()) {
            vec->erase(vec->begin() + index);
        }
        
    }
    stalePU->clear();

}

int* getRandomPosition()
{
    static int pos[2];
    pos[0] = rand() % (FIELD_WIDTH - 50) + field_rect.x + 25;
    pos[1] = rand() % (FIELD_HEIGHT - 50) + field_rect.y + 25;
    return pos;
}


void TwoPlayerMode(SDL_Surface* screen_surface, SDL_Surface* image_surface)
{
    // Main game loop for 2 player mode
    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;

    //Set text color as black
    SDL_Color textColor = { 0, 0, 0, 255 };

    //Timers
    Uint32 startTime = 0;
    Uint32 player1Timers[2] = {0,0};
    Uint32 player2Timers[2] = {0,0};
    Uint32 powerup1Duration = 10000; // 10 seconds
    Uint32 powerup2Duration = 15000; // 15 seconds

    //In memory text stream
    std::stringstream timeText;

    //balls
    Paddle player1Paddle(true);
    Paddle player2Paddle(false);
    SDL_Rect* tempRect = NULL;
    SDL_Rect* player1ScoreClipDec = NULL;
    SDL_Rect* player1ScoreClipTen = NULL;
    SDL_Rect* player2ScoreClipDec = NULL;
    SDL_Rect* player2ScoreClipTen = NULL;

    int player1Score = 0;
    int player2Score = 0;
    int index;
    int ballNum;
    bool spawnBall = false;
    Ball tempBall = Ball(0,0,0,0,0);
    std::vector<int> staleBalls;
    Powerup tempPU = Powerup(0,0,0);
    std::vector<int> stalePU;
    std::vector<Ball> balls;
    std::vector<Powerup> powerups;
    
    int collision = NO_COLLISION;
    int lastCollision = NO_COLLISION;
    int* randPos;
    int direction = 1;

    powerups.push_back(initPowerup(25 + SCREEN_WIDTH/2,25 + SCREEN_HEIGHT/2, 0));

    //While application is running
    while( !quit )
    {

        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
                printf("Quitting...\n");
            }
            else if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_q)
            {
                quit = true;
                printf("Quitting...\n");
            }
            else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_1 )
            {
                randPos = getRandomPosition();
                powerups.push_back(initPowerup(randPos[0], randPos[1], 0));
            }
            else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_2 )
            {
                randPos = getRandomPosition();
                powerups.push_back(initPowerup(randPos[0], randPos[1], 1));
            }
            else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_3 )
            {
                randPos = getRandomPosition();
                powerups.push_back(initPowerup(randPos[0], randPos[1], 2));
            }

        player1Paddle.handleEvent ( e );
        player2Paddle.handleEvent ( e );
        }

        // Handle powerup durations
        if(player1Paddle.move_powerup && (SDL_GetTicks() - player1Timers[0] >= powerup1Duration))
        {
            player1Paddle.move_powerup = false;
            player1Paddle.mPosX = field_rect.x + 2*BAR_WIDTH;
            player1Paddle.collisionRect.x = player1Paddle.mPosX;
        }
        if(player2Paddle.move_powerup && (SDL_GetTicks() - player2Timers[0] >= powerup1Duration))
        {
            player2Paddle.move_powerup = false;
            player2Paddle.mPosX = field_rect.x + field_rect.w - 3*BAR_WIDTH;
            player2Paddle.collisionRect.x = player2Paddle.mPosX;
        }


        if (balls.empty())
        {
            direction = lastCollision == RIGHT_GOAL_COLLISION ? 1 : -1;   
            balls.push_back(initBall(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 5*direction, 0, lastCollision == RIGHT_GOAL_COLLISION));
        
        }

        ballNum = balls.size();
        for(int i = 0; i < ballNum; i++)
        {
            collision = balls[i].move(player1Paddle, player2Paddle, lastCollision);
            if (collision == RIGHT_GOAL_COLLISION)
            {
                player1Score++;
                staleBalls.push_back(i);

            } else if (collision == LEFT_GOAL_COLLISION)
            {
                player2Score++;
                staleBalls.push_back(i);
            }
            for(int j = 0; j < powerups.size(); j++){
                if(powerups.at(j).checkCollision(balls.at(i).mCollider)){
                    
                    collision = POWERUP_COLLISION;
                    stalePU.push_back(j);
                    switch(powerups.at(j).power){
                        case 0:
                            if(balls.at(i).blue){
                                player1Paddle.move_powerup = true;
                                player1Timers[0] = SDL_GetTicks();
                            }
                            else{
                                player2Paddle.move_powerup = true;
                                player2Timers[0] = SDL_GetTicks();
                            }
                            break;
                        case 1:      
                            direction = balls.at(i).blue ? 1 : -1;
                            balls.push_back(initBall(powerups.at(j).mPosX, powerups.at(j).mPosY, 5*direction, 0, balls.at(i).blue));
                            //balls.push_back(initBall(powerups.at(j).mPosX, powerups.at(j).mPosY, 5*direction, 5, balls.at(i).blue));
                            //balls.push_back(initBall(powerups.at(j).mPosX, powerups.at(j).mPosY, 5*direction, -5, balls.at(i).blue));
                            
                            
                            break;
                        case 2:      
                            if(balls.at(i).blue){
                                PADDLE_HEIGHT = 100;
                                player1Paddle.collisionRect.h = PADDLE_HEIGHT;
                                player1Timers[1] = SDL_GetTicks();
                            }
                            else{
                                PADDLE_HEIGHT = 100;
                                player2Paddle.collisionRect.h = PADDLE_HEIGHT;
                                player2Timers[1] = SDL_GetTicks();
                            }
                            break;                               
                            
                    }
                    
                    powerups.at(j).power = -1; // Ensure powerup doesn't trigger multiple times  
                    
                }
            }
        }
        // Remove stale balls
        DeleteBalls(&balls, &staleBalls);
        
        // Remove duplicate indices in case multiple balls hit a powerup in one frame
        sort(stalePU.begin(), stalePU.end());
        auto it = unique(stalePU.begin(), stalePU.end()); // Move all duplicates to last of vector
        stalePU.erase(it, stalePU.end()); // Remove all duplicates
        // Remove stale powerups
        DeletePowerups(&powerups, &stalePU);
        
        if(collision != PLAYER1_COLLISION)
        {
            player1Paddle.move();
            
        }
        if(collision != PLAYER2_COLLISION)
        {
            player2Paddle.move();
            
        }


        //Set text to be rendered
        timeText.str( "" );
        timeText << "Time " << ((SDL_GetTicks() - startTime) / 1000)/60 << ":" << ((SDL_GetTicks() - startTime) / 1000) % 60;

        //Render text
        if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
        {
            printf( "Unable to render time texture!\n" );
        }

        
        //Clear screen
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );
        //SDL_RenderFillRect( renderer, &field_rect );

        //Render textures
        gTimeTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, textTexture.getHeight() );

        drawField(renderer, &field_rect);
        for(int i = 0; i < balls.size(); i++)
        {
            balls[i].render();
        } 
        for(int i = 0; i < powerups.size(); i++)
        {
            tempPU = powerups[i];
            if(tempPU.power != 3){
                tempRect = &powerupClips[tempPU.power];
                powerupTexture.render(tempPU.mCollider.x, tempPU.mCollider.y, tempRect);
            }
            
        }
        player1Paddle.render();
        player2Paddle.render();
        
        //Render current frame
        textTexture.render( (SCREEN_WIDTH - textTexture.getWidth())/2, 0);
        
        player1ScoreClipDec = &spriteClips[ player1Score%10 ];
        player1ScoreClipTen = &spriteClips[ player1Score/10 ];
        player2ScoreClipDec = &spriteClips[ player2Score%10 ];
        player2ScoreClipTen = &spriteClips[ player2Score/10 ];
        playerScoreTexture.render( 0, 0, player1ScoreClipTen );
        playerScoreTexture.render( player1ScoreClipDec->w, 0, player1ScoreClipDec );
        playerScoreTexture.render(SCREEN_WIDTH - 2*player2ScoreClipTen->w, 0, player2ScoreClipTen);
        playerScoreTexture.render(SCREEN_WIDTH - player2ScoreClipDec->w, 0, player2ScoreClipDec);
        

        //SDL_BlitSurface( image_surface, NULL, screen_surface, NULL );
        //Update the surface and screen
        SDL_RenderPresent( renderer );
        //SDL_UpdateWindowSurface( window );
        
        lastCollision = collision;
    }
}


int main( int argc, char* args[] )
{
    SDL_Surface* screen_surface = NULL;
    SDL_Surface* image_surface = NULL;

    

    SDL_RenderSetViewport( renderer, &field_rect);
    //Start up SDL and create window
    if( !initWindow(&window,&screen_surface,&renderer) )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia(&image_surface) || !loadTextMedia(&textTexture, "MegaPong") || !loadNumbers() || !loadPowerups())        
        {
            printf( "Failed to load media!\n" );
        }
        else
        {   
            //Apply the image
            //SDL_BlitSurface( image_surface, NULL, screen_surface, NULL );
            TwoPlayerMode(screen_surface, image_surface);

            
            
        }
    }

    //Free resources and close SDL
    closeWindow(&image_surface);

    return 0;
}

