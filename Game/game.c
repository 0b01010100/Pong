#include <SDL3/SDL.h>
#include <game.h>
#include <stdio.h>
typedef SDL_FRect wall, ball, paddle;
typedef struct{
    wall left;
    wall top;
    wall right;
    wall bottom;
    paddle paddle; 
    float thickness;
    ball ball; 
}scene;
typedef struct _game{
    scene scene;
    SDL_Window * window;
    SDL_Renderer * renderer;
    Uint64 lastTime;
    Uint64 currentTime;
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
        printf("d");
        return false;        
    }

    //Game creation
    game * game = malloc(sizeof(struct _game));

    //window creation
    game->window = SDL_CreateWindow(
        "Pong Game",
        1024,   //Width 
        768,    //Height
        100,    //Pos X
        100,    //Pos Y
        SDL_WINDOW_RESIZABLE       //Flags
    );

    if(!game->window)
    {
        SDL_Log("Failed to create game window: %s", SDL_GetError());
        // Clean up on failure
        free(game); 
        return false;
    }

    //renderer creation
    game->renderer = SDL_CreateRenderer
    (
        game->window,   //window
        NULL
    );
    if(!game->renderer)
    {
        SDL_Log("Failed to create game renderer: %s", SDL_GetError());
        // Clean up on failure
        SDL_DestroyWindow(game->window); 
        free(game);
        return false;
    }
    game->scene.thickness = 15;
    game->scene.paddle.h = 100;
    game->scene.paddle.w = game->scene.thickness;
    game->lastTime = SDL_GetTicks();
    game->isRunning = true;
    return game;
}

bool game_ProcessInput(game *game)
{
    SDL_Event event;
    if(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            game->isRunning = false;
            break;
        }
    }
    
    //get keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_ESCAPE])
    {
        game->isRunning = false;
    }
    if(state[SDL_SCANCODE_W])
    {
        game->scene.paddle.y -= 2;
    }
    if(state[SDL_SCANCODE_S])
    {
        game->scene.paddle.y += 2;
    }
    return game->isRunning;
}

#define FRAME_RATE 60
#define FRAME_DELAY (1000 / FRAME_RATE)

void game_Update(game *game)
{
    game->currentTime = SDL_GetTicks();
    game->deltaTime = (game->currentTime - game->lastTime) / 1000.0f;
    game->lastTime = game->currentTime;
    
    SDL_RenderClear(game->renderer);
    SDL_SetRenderDrawColor
    (
        game->renderer, // renderer
        0, 
        0,
        255,
        255
    );
    int width, height;
    SDL_GetWindowSize(game->window, &width, &height);

    //top
    game->scene.top.w = width;
    game->scene.top.h = 15;
    SDL_RenderFillRect(game->renderer, &game->scene.top);
    //right
    game->scene.right.w = 15;
    game->scene.right.h = height-30;
    game->scene.right.y = 15;
    game->scene.right.x = width - 15;
    SDL_RenderFillRect(game->renderer, &game->scene.right);
    //bottom
    game->scene.bottom.w = width;
    game->scene.bottom.h = 15;
    game->scene.bottom.y = height-15;
    SDL_RenderFillRect(game->renderer, &game->scene.bottom);
    //paddle
    SDL_SetRenderDrawColor
    (
        game->renderer, // renderer
        255, // red
        0, // blue
        0, //green
        0  //alpha
    );
    paddle p = game->scene.paddle;
    p.y = p.y - p.h/2; 
    SDL_RenderFillRect(game->renderer, &p);
    //ball

    SDL_SetRenderDrawColor
    (
        game->renderer, // renderer
        0,// red
        0,// blue
        0,//green
        0 //alpha
    );
    
    //show drawing
    SDL_RenderPresent(game->renderer);
   
    Uint32 frameTime = SDL_GetTicks() - game->currentTime;

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
