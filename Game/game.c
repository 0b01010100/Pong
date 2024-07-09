#include <SDL2/SDL.h>
#include <game.h>
#include <stdio.h>
typedef SDL_Rect wall, ball, paddle;
#define thickness 15
typedef struct{
    wall left;
    wall top;
    wall right;
    wall bottom;
    paddle paddle; 
    ball ball; 
    float paddleDir;
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
        SDL_WINDOWPOS_CENTERED,   //Width 
        SDL_WINDOWPOS_CENTERED,    //Height
        1024,    //Pos X
        768,    //Pos Y
        0       //Flags
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
        0               //NULL flages
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
    int width, height;
    SDL_GetWindowSize(game->window, &width, &height);
    game->scene.paddle.y = (height / 2) - (game->scene.paddle.h / 2);// Center paddle vertically
    game->scene.paddle.x = 17;//Initial X position of the paddle
    //Set initial properties for the paddle
    game->scene.ball.h = thickness;
    game->scene.ball.w = thickness;
    game->scene.ball.y = (height / 2);
    game->scene.ball.x = (width / 2);
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

    //clear buffer
    SDL_RenderClear(game->renderer);

    //change color to draw with
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255);

    //get windpw width and height
    int width, height;
    SDL_GetWindowSize(game->window, &width, &height);

    //draw top boarder
    game->scene.top.w = width;
    game->scene.top.h = thickness;
    game->scene.top.x = 0;
    game->scene.top.y = 0;
    SDL_RenderFillRect(game->renderer, &game->scene.top);

    //draw right board
    game->scene.top.w = thickness;
    game->scene.top.h = height;
    game->scene.top.x = width - thickness;
    game->scene.top.y = 0;
    SDL_RenderFillRect(game->renderer, &game->scene.top);

    //draw bottom board
    game->scene.top.w = width;
    game->scene.top.h = thickness;
    game->scene.top.x = 0;
    game->scene.top.y = height - thickness;
    SDL_RenderFillRect(game->renderer, &game->scene.top);
    
    //change color to draw with
    SDL_SetRenderDrawColor(game->renderer,255, 0, 0, 255);

    //draw paddle
    //check for paddle collisions on the top boarder 
    game->scene.paddle.y += (float)game->scene.paddleDir * game->deltaTime * 300.0f;
    if(game->scene.paddle.y < 0 + thickness){
        game->scene.paddle.y = 0 + thickness;
    }
    //check for paddle collisions on the bottom boarder 
    else if(game->scene.paddle.y > height - thickness - 100){
        game->scene.paddle.y = (height - thickness) - 100;
    }
    SDL_RenderFillRect(game->renderer, &game->scene.paddle);

    //change color to draw with
    SDL_SetRenderDrawColor(game->renderer,0,255,0,0);
    
    SDL_RenderFillRect(game->renderer, &game->scene.ball);
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
