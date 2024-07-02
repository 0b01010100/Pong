#include <game.h>

int main()
{
    game * game = 0;
    if(!game_Construct(game))
    {
        return -1;
    }
    while(game_ProcessInput(game)) 
    {
        game_Update(game);
    }
    game_Destruct(game);
    return 0;
}