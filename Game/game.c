#include <SDL3/SDL.h>
#include <game.h>

typedef struct _game{
    SDL_Window * window;
    bool isRunning;
}game;

bool game_Construct(game *game)
{
    int adlResualt = SDL_Init(SDL_INIT_VIDEO);//init Graphics Engine
    if (adlResualt != 0)
    {
        SDL_Log("faild To Init SDL");
        return false;        
    }
    game = malloc(sizeof(struct _game));
    game->window = SDL_CreateWindow(
        "Pong Game",
        1024,    //Pos X
        768,    //Pos Y
        100,   //Width
        100,    //Height
        SDL_WINDOW_RESIZABLE       //Flags
    );
    if(!game->window)
    {
        SDL_Log("faild To Create Game Window: $s", SDL_GetError()); 
        return false;
    }
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
    return true;

    //get keyboard state
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if(state[SDL_SCANCODE_ESCAPE])
    {
        game->isRunning = false;
    }
}

void game_Update(game *game)
{
}

void game_Destruct(game *game)
{
    SDL_DestroyWindow(game->window);
    SDL_Quit();//uninit SDL
}
