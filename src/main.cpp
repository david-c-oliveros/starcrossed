#include "game.h"


Game game((int)(1920 * 1.0), (int)(1080 * 1.0));


int main()
{
    game.Create();

    while(game.Update()) {}

    return 0;
}
