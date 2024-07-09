#include <game.h>

int main()
{
        game * game = game_Construct();
        if(!game)
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