#include <SDL2/SDL.h>
#include <game.h>
#include <stdio.h>
typedef SDL_Rect wall, ball, paddle;
#define thickness 15
typedef struct{
    paddle paddle; 
    ball ball; 
    float paddleDir;
    float mBallVelx;
    float mBallVely;
}scene;
typedef struct _game{
    scene scene;
    SDL_Window * window;
    SDL_Renderer * renderer;
    Uint32 lastTime;
    Uint32 currentTime;
    float deltaTime;
    bool isRunning;

}game;

game * game_Construct()
{
    //Initialize SDL
    int adlResualt = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER);
    if (adlResualt != 0)
    {
        SDL_Log("faild To Init SDL");
        return false;        
    }

    //Game creation
    game * game = malloc(sizeof(struct _game));

    //window creation
    game->window = SDL_CreateWindow(
        "Pong Game",
        SDL_WINDOWPOS_CENTERED,  //Pos X 
        SDL_WINDOWPOS_CENTERED,  //Pos Y
        1024,   //Width 
        768,   //Height
        0U        //Flags
    );

    if(!game->window)
    {
        SDL_Log("Failed to create game window: %s", SDL_GetError());
        // Clean up on failure
        free(game); 
        SDL_Quit();//uninit SDL
        return false;
    }

    //renderer creation
    game->renderer = SDL_CreateRenderer
    (
        game->window,   //Window
        -1,             //Index of rendering driver
        0U               //NULL flages
    );
    if(!game->renderer)
    {
        SDL_Log("Failed to create game renderer: %s", SDL_GetError());
        // Clean up on failure
        SDL_DestroyWindow(game->window); 
        free(game);
        SDL_Quit();//uninit SDL
        return false;
    }
    
    //Set initial properties for the paddle
    game->scene.paddle.h = 100;// Height of the paddle
    game->scene.paddle.w = thickness;// Width of the paddle 
    //get widow width and height
    game->scene.paddle.y = 1024/2;// Center paddle vertically
    game->scene.paddle.x = 10;//Initial X position of the paddle
    //Set initial properties for the paddle
    game->scene.ball.h = thickness;
    game->scene.ball.w = thickness;
    game->scene.ball.y = (1024 / 2) - thickness;
    game->scene.ball.x = (768 / 2) - thickness;
    game->scene.mBallVelx = -200.0f;
    game->scene.mBallVely = 235.0f;
    game->lastTime = SDL_GetTicks();//Initialize lastTime with current SDL ticks
    game->isRunning = true; //Set game state to running
    return game;
}

bool game_ProcessInput(game *game)
{
    SDL_Event event;
    if(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            game->isRunning = false;
            break;
        }
    }
    
    //get keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);
    game->scene.paddleDir = 0.0f;
    if(state[SDL_SCANCODE_ESCAPE])
    {
        game->isRunning = false;
    }
    if(state[SDL_SCANCODE_W])
    {
        game->scene.paddleDir -= 2.0f;
    }
    if(state[SDL_SCANCODE_S])
    {
        game->scene.paddleDir += 2.0f;
    }
    return game->isRunning;
}

#define FRAME_RATE 60
#define FRAME_DELAY (1000 / FRAME_RATE)

void game_Update(game *game)
{
    // Get the current time in milliseconds since SDL initialization
    game->currentTime = SDL_GetTicks();
    // Calculate deltaTime (time elapsed since last frame) in seconds
    game->deltaTime = (game->currentTime - game->lastTime) / 1000.0f;
    // Update lastTime to current time for the next frame calculation
    game->lastTime = game->currentTime;

    // limit the delta time to a max value
    if (game->deltaTime > 0.05f)
	{
		game->deltaTime = 0.05f;
	}

    //clear buffer
    SDL_RenderClear(game->renderer);

    //change color to draw with
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);

    //get windpw width and height
    int width, height;
    SDL_GetWindowSize(game->window, &width, &height);

    wall Wall = (wall){
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
    //draw top boarder
    SDL_RenderFillRect(game->renderer, &Wall);
    
    //draw bottom board
    Wall.y = 768 - thickness;
    SDL_RenderFillRect(game->renderer, &Wall);

    //draw right board
	Wall.x = 1024 - thickness;
	Wall.y = 0;
	Wall.w = thickness;
	Wall.h = 1024;
    SDL_RenderFillRect(game->renderer, &Wall);
    
    //change color to draw with
    SDL_SetRenderDrawColor(game->renderer,255, 0, 0, 255);

    //draw paddle
    //check for paddle collisions on the top boarder 
    if (game->scene.paddleDir != 0)
    {
        game->scene.paddle.y += (int)(game->scene.paddleDir * game->deltaTime * 300.0f);
        if(game->scene.paddle.y < game->scene.paddle.h / 2 + thickness){
            game->scene.paddle.y = game->scene.paddle.h / 2 + thickness;
        }
        //check for paddle collisions on the bottom boarder 
        else if(game->scene.paddle.y > (height - (game->scene.paddle.h / 2) ) - thickness){
            game->scene.paddle.y = (height - (game->scene.paddle.h / 2) ) - thickness;
        }
    }
    paddle p = game->scene.paddle;
    p.y -= p.h/2;
    SDL_RenderFillRect(game->renderer, &p);

    //change color to draw with
    SDL_SetRenderDrawColor(game->renderer,0,255,0,0);
    
    // Use velocity to update ball position
    game->scene.ball.x += (int)(game->scene.mBallVelx * game->deltaTime);
    game->scene.ball.y += (int)(game->scene.mBallVely * game->deltaTime);

    //Bounce ball
    int diff = (game->scene.paddle.y - game->scene.ball.y);
    diff = (diff > 0) ? diff : -diff;
    if
    (
        diff <= game->scene.paddle.h / 2 &&
        game->scene.ball.x <= 25 && game->scene.ball.x >= 20 &&
        game->scene.mBallVelx < 0
    )
    {
        game->scene.mBallVelx *= -1;
    }
    //game over if ball when past the left side of the window
    else if(game->scene.ball.x <= 0)
    {
        game->isRunning = false;
    }
    //check if ball hit the right boarder if so bouce it back
    else if(game->scene.ball.x >= (1024 - thickness) && game->scene.mBallVelx > 0)
    {
        game->scene.mBallVelx *= -1;
    }
    
    //check if ball hit the top boarder if so bouce it back?
	if (game->scene.ball.y <= thickness)
	{
		game->scene.mBallVely *= -1;
	}
    else if(game->scene.ball.y >= (768 - thickness) && game->scene.mBallVely > 0)
    {
        game->scene.mBallVely *= -1;
    }
    ball b = game->scene.ball;
    b.x -= thickness/2;
    b.y -= thickness/2;
    SDL_RenderFillRect(game->renderer, &b);

    //change color to draw with
    SDL_SetRenderDrawColor(game->renderer,0,0,0,0);
    
    //show drawings
    SDL_RenderPresent(game->renderer);
   
    //get time spent of this frame
    Uint32 frameTime = SDL_GetTicks() - game->currentTime;

    //dealy the next frame basied on the difference between FRAME_DELAY and frame time 
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }
}

void game_Destruct(game *game)
{
    if (game)
    {
        SDL_DestroyRenderer(game->renderer);
        SDL_DestroyWindow(game->window);
        free(game);
        SDL_Quit();//uninit SDL
    }
}
