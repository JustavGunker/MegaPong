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
LTexture menuTextTexture[9];
SDL_Rect menuRects[9];
SDL_Rect spriteClips[ 10 ];
SDL_Rect powerupClips[ 3 ];
SDL_Rect powerupDecayClips[ 10 ];
LTexture playerScoreTexture;
LTexture powerupTexture;
LTexture powerupDecayTexture;
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

bool loadPowerupDecay()
{
    //Loading success flag
    bool success = true;

    //Load sprite sheet texture
    if( !powerupDecayTexture.loadFromFile( "powerupdecay.png" ))
    {
        printf( "Failed to load powerup texture!\n" );
        success = false;
    }
    else
    {   
        for(int i = 0; i < 5; i++) 
        {
            
            //Set sprite clips
            powerupDecayClips[ i ].x = 0;
            powerupDecayClips[ i ].y = i*22;
            powerupDecayClips[ i ].w = 23;
            powerupDecayClips[ i ].h = 22;
            
        }
        for(int i = 0; i < 5; i++) 
        {
            
            //Set sprite clips
            powerupDecayClips[ i+5 ].x = 23;
            powerupDecayClips[ i+5 ].y = i*22;
            powerupDecayClips[ i+5 ].w = 23;
            powerupDecayClips[ i+5 ].h = 22;
            
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
    powerupDecayTexture.free();

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

void ExtendPaddle(int addHeight, Paddle * playerPaddle)
{
    int newPaddleHeight = PADDLE_HEIGHT + addHeight;
    playerPaddle->mHeight = newPaddleHeight;
    playerPaddle->collisionRect.h = newPaddleHeight;

    if(playerPaddle->mPosY > field_rect.y + FIELD_HEIGHT/2){
        playerPaddle->mPosY -= addHeight;
    } 
}

void RetractPaddle(Paddle * playerPaddle)
{
    int newPaddleHeight = PADDLE_HEIGHT;
    playerPaddle->mHeight = newPaddleHeight;
    playerPaddle->collisionRect.h = newPaddleHeight;
}

int GetDecayIndex(Uint32 startTime, Uint32 totalDuration) {
    Uint32 elapsedTime = SDL_GetTicks() - startTime;

    // Clamp to avoid going beyond totalDuration
    if (elapsedTime > totalDuration)
        elapsedTime = totalDuration;

    // Compute which fourth we’re in
    int index = (elapsedTime * 4) / totalDuration;

    // Clamp again in case of rounding (e.g., exactly totalDuration)
    if (index > 3)
        index = 3;

    return index;
}

void loadMenuText(LTexture* textTexture,int x, int y, std::string string){
//Set text to be rendered
    std::stringstream stream;
    stream.str( "" );
    stream << string;

    SDL_Color textColor = { 0, 0, 0, 255 };

    //Render text
    if( !textTexture->loadFromRenderedText( stream.str().c_str(), textColor ) )
    {
        printf( "Unable to render menu texture!\n" );
    }

}

bool checkMouseCollision( SDL_Rect* a, int mouseX, int mouseY)
{
    SDL_Rect b = {mouseX, mouseY, 1, 1};
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    

    //Calculate the sides of rect A
    leftA = a->x;
    rightA = a->x + a->w;
    topA = a->y;
    bottomA = a->y + a->h;

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


void spawnPowerupRandomly(std::vector<Powerup>* powerups){
    int* randPos = getRandomPosition();
    int randType = rand() % 3; // Assuming 3 types of powerups
    powerups->push_back(initPowerup(randPos[0], randPos[1], randType));
}

bool pauseScreen(){

    bool quit = false;

    //Load menu buttons
    loadMenuText(&menuTextTexture[6] , SCREEN_WIDTH/2, SCREEN_HEIGHT/3, "PAUSED");
    menuRects[6] = {SCREEN_WIDTH/2 - menuTextTexture[6].getWidth()/2, SCREEN_HEIGHT/3 - menuTextTexture[6].getHeight()/2, menuTextTexture[6].getWidth(), menuTextTexture[6].getHeight()};
    loadMenuText(&menuTextTexture[7] , SCREEN_WIDTH/3, 2*SCREEN_HEIGHT/3, "Exit");
    menuRects[7] = {SCREEN_WIDTH/3 - menuTextTexture[7].getWidth()/2, 2*SCREEN_HEIGHT/3 - menuTextTexture[7].getHeight()/2, menuTextTexture[7].getWidth(), menuTextTexture[7].getHeight()};
    loadMenuText(&menuTextTexture[8] , 2*SCREEN_WIDTH/3, 2*SCREEN_HEIGHT/3, "Continue");
    menuRects[8] = {2*SCREEN_WIDTH/3 - menuTextTexture[8].getWidth()/2, 2*SCREEN_HEIGHT/3 - menuTextTexture[8].getHeight()/2, menuTextTexture[8].getWidth(), menuTextTexture[8].getHeight()};


    //Event handler
    SDL_Event e;
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
                else if (e.type == SDL_MOUSEBUTTONUP){
                    
                    // Get mouse position
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    if(checkMouseCollision(&menuRects[7], x, y)){
                        return true;
                    } else if(checkMouseCollision(&menuRects[8], x, y)){
                        return false;
                    }

                }
            }
            // Clear and render buttons
            SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( renderer );

            menuTextTexture[6].render( menuRects[6].x, menuRects[6].y );
            menuTextTexture[7].render( menuRects[7].x, menuRects[7].y );
            menuTextTexture[8].render( menuRects[8].x, menuRects[8].y );

            
            SDL_RenderPresent( renderer );
        }
    return false;
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
    Uint32 startTime = SDL_GetTicks();
    Uint32 player1Timers[2] = {0,0};
    Uint32 player2Timers[2] = {0,0};
    Uint32 powerup1Duration = 10000; // 10 seconds
    Uint32 powerup2Duration = 15000; // 15 seconds
    Uint32 powerupTimer = 0; // Random time between 2 and 5 seconds
    Uint32 gameDelay = 3000;

    //In memory text stream
    std::stringstream timeText;


    
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

    // Short delay for players to get ready
    while( SDL_GetTicks() - startTime < gameDelay ){
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );
        drawField(renderer, &field_rect);
        
        //Set text to be rendered
        timeText.str( "" );
        timeText <<  (gameDelay - (SDL_GetTicks() - startTime))/1000 + 1;

        //Render text
        if( !gTimeTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor ) )
        {
            printf( "Unable to render time texture!\n" );
        }

        
        //Render textures
        gTimeTextTexture.render( ( SCREEN_WIDTH - gTimeTextTexture.getWidth() ) / 2, textTexture.getHeight() );


        SDL_RenderPresent( renderer );
    }

    startTime = SDL_GetTicks(); // Reset start time for actual game
    powerupTimer = SDL_GetTicks() - startTime + rand() % 5000 + 10000;

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
            else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE )
            {
                Uint32 pauseTime = SDL_GetTicks();
                quit = pauseScreen();
                startTime += SDL_GetTicks() - pauseTime; // Adjust start time to account for pause duration
                powerupTimer = SDL_GetTicks() - startTime + rand() % 5000 + 10000; // Reset powerup timer
            }
            else if( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_0 ) // Spawn random powerup for testing
            {
                randPos = getRandomPosition();
                powerups.push_back(initPowerup(randPos[0], randPos[1], 2));
            }

        player1Paddle.handleEvent ( e );
        player2Paddle.handleEvent ( e );
        }

        // Handle powerup durations
        if(player1Paddle.powerups[0] && (SDL_GetTicks() - player1Timers[0] >= powerup1Duration))
        {
            player1Paddle.powerups[0] = false;
            player1Paddle.mPosX = field_rect.x + 2*BAR_WIDTH;
            player1Paddle.collisionRect.x = player1Paddle.mPosX;
        }
        if(player2Paddle.powerups[0] && (SDL_GetTicks() - player2Timers[0] >= powerup1Duration))
        {
            player2Paddle.powerups[0] = false;
            player2Paddle.mPosX = field_rect.x + field_rect.w - 3*BAR_WIDTH;
            player2Paddle.collisionRect.x = player2Paddle.mPosX;
        }

        if(player1Paddle.powerups[1] &&(SDL_GetTicks() - player1Timers[1] >= powerup2Duration))
        {
            player1Paddle.powerups[1] = false;
            RetractPaddle(&player1Paddle);
        }
        if(player2Paddle.powerups[1] &&(SDL_GetTicks() - player2Timers[1] >= powerup2Duration))
        {
            player2Paddle.powerups[1] = false;
            RetractPaddle(&player2Paddle);
        }
        // Handle spawning powerups
        if(SDL_GetTicks() - startTime >= powerupTimer){
            spawnPowerupRandomly(&powerups);
            powerupTimer = SDL_GetTicks() - startTime + rand() % 5000 + 10000; // Reset timer
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
                                player1Paddle.powerups[0] = true;
                                player1Timers[0] = SDL_GetTicks();
                            }
                            else{
                                player2Paddle.powerups[0] = true;
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
                                ExtendPaddle(40, &player1Paddle);
                                player1Paddle.powerups[1] = true;
                                player1Timers[1] = SDL_GetTicks();
                            }
                            else{
                                ExtendPaddle(40, &player2Paddle);
                                player2Paddle.powerups[1] = true;
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
        
        if(player1Paddle.powerups[0]){
            tempRect = &powerupDecayClips[GetDecayIndex(player1Timers[0],powerup1Duration)];
            powerupDecayTexture.render(tempRect->w, player1ScoreClipDec->h, tempRect);
        }
        if(player1Paddle.powerups[1]){
            tempRect = &powerupDecayClips[5 + GetDecayIndex(player1Timers[1],powerup2Duration)];
            powerupDecayTexture.render(2*tempRect->w, player1ScoreClipDec->h, tempRect);
        }
        if(player2Paddle.powerups[0]){
            tempRect = &powerupDecayClips[GetDecayIndex(player2Timers[0],powerup1Duration)];
            powerupDecayTexture.render(SCREEN_WIDTH - 3*tempRect->w, player2ScoreClipDec->h, tempRect);
        }
        if(player2Paddle.powerups[1]){
            tempRect = &powerupDecayClips[5 + GetDecayIndex(player2Timers[1],powerup2Duration)];
            powerupDecayTexture.render(SCREEN_WIDTH - 2*tempRect->w, player2ScoreClipDec->h, tempRect);
        }

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

    //Main loop flag
    bool quit = false;
    bool singlePlayerMode = false;
    SDL_Rect* mouseRect;

  

    //Event handler
    SDL_Event e;


    

    SDL_RenderSetViewport( renderer, &field_rect);
    //Start up SDL and create window
    if( !initWindow(&window,&screen_surface,&renderer) )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia(&image_surface) || !loadTextMedia(&textTexture, "MegaPong") || !loadNumbers() || !loadPowerups() || !loadPowerupDecay())        
        {
            printf( "Failed to load media!\n" );
        }
        else
        {   
            //Load menu buttons
            loadMenuText(&menuTextTexture[0] , SCREEN_WIDTH/2, SCREEN_HEIGHT/3, "1 Player");
            menuRects[0] = {SCREEN_WIDTH/2 - menuTextTexture[0].getWidth()/2, SCREEN_HEIGHT/3 - menuTextTexture[0].getHeight()/2, menuTextTexture[0].getWidth(), menuTextTexture[0].getHeight()};
            loadMenuText(&menuTextTexture[1] , SCREEN_WIDTH/2, 2*SCREEN_HEIGHT/3, "2 Players");
            menuRects[1] = {SCREEN_WIDTH/2 - menuTextTexture[1].getWidth()/2, 2*SCREEN_HEIGHT/3 - menuTextTexture[1].getHeight()/2, menuTextTexture[1].getWidth(), menuTextTexture[1].getHeight()};
            loadMenuText(&menuTextTexture[2] , SCREEN_WIDTH/2, SCREEN_HEIGHT/4, "Easy");
            menuRects[2] = {SCREEN_WIDTH/2 - menuTextTexture[2].getWidth()/2, SCREEN_HEIGHT/4 - menuTextTexture[2].getHeight()/2, menuTextTexture[2].getWidth(), menuTextTexture[2].getHeight()};
            loadMenuText(&menuTextTexture[3] , SCREEN_WIDTH/2, 2*SCREEN_HEIGHT/4, "Medium");
            menuRects[3] = {SCREEN_WIDTH/2 - menuTextTexture[3].getWidth()/2, 2*SCREEN_HEIGHT/4 - menuTextTexture[3].getHeight()/2, menuTextTexture[3].getWidth(), menuTextTexture[3].getHeight()};
            loadMenuText(&menuTextTexture[4] , SCREEN_WIDTH/2, 3*SCREEN_HEIGHT/4, "Hard");
            menuRects[4] = {SCREEN_WIDTH/2 - menuTextTexture[4].getWidth()/2, 3*SCREEN_HEIGHT/4 - menuTextTexture[4].getHeight()/2, menuTextTexture[4].getWidth(), menuTextTexture[4].getHeight()};
            loadMenuText(&menuTextTexture[5] , 0, SCREEN_HEIGHT - menuTextTexture[5].getHeight(), "Back");
            menuRects[5] = {0, SCREEN_HEIGHT - menuTextTexture[5].getHeight(), menuTextTexture[5].getWidth(), menuTextTexture[5].getHeight()};

            
            
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
                    else if (e.type == SDL_MOUSEBUTTONUP){
                        
                        // Get mouse position
                        int x, y;
                        SDL_GetMouseState(&x, &y);

                        if(singlePlayerMode){
                            if(checkMouseCollision(&menuRects[2], x, y)){
                                printf("Easy mode selected\n");
                                //SinglePlayerMode(screen_surface, image_surface);
                            }
                            else if(checkMouseCollision(&menuRects[3], x, y)){
                                printf("Medium mode selected\n");
                                //SinglePlayerMode(screen_surface, image_surface);
                            }
                            else if(checkMouseCollision(&menuRects[4], x, y)){
                                printf("Hard mode selected\n");
                                //SinglePlayerMode(screen_surface, image_surface);
                            }
                            else if(checkMouseCollision(&menuRects[5], x, y)){
                                singlePlayerMode = false;
                            }
                        } else {
                            if(checkMouseCollision(&menuRects[0], x, y)){
                                printf("1 Player mode selected\n");
                                //SinglePlayerMode(screen_surface, image_surface);
                                singlePlayerMode = true;
                            }
                            else if(checkMouseCollision(&menuRects[1], x, y)){
                                printf("2 Player mode selected\n");
                                TwoPlayerMode(screen_surface, image_surface);
                            }
                        }

                    }
                }
                // Clear and render buttons
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( renderer );
                if(singlePlayerMode){
                    menuTextTexture[2].render( menuRects[2].x, menuRects[2].y );
                    menuTextTexture[3].render( menuRects[3].x, menuRects[3].y );
                    menuTextTexture[4].render( menuRects[4].x, menuRects[4].y );
                    menuTextTexture[5].render( menuRects[5].x, menuRects[5].y );
                } else {
                    menuTextTexture[0].render( menuRects[0].x, menuRects[0].y );
                    menuTextTexture[1].render( menuRects[1].x, menuRects[1].y );
                }
                
                SDL_RenderPresent( renderer );
            }
            
        }
    }


    

    //Free resources and close SDL
    closeWindow(&image_surface);

    return 0;
}

