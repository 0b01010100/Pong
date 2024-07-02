#ifndef game_H
#define game_H
#include <stdbool.h>
typedef struct _game game;

game * game_Construct();

bool game_ProcessInput(game * game);

void game_Update(game * game);

void game_Destruct(game * game);

#endif // game_H