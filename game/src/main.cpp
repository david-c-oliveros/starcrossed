#include "app.h"


//Game game((int)(1920 * 1.0), (int)(1080 * 1.0));
App game((int)(2000 * 1.0), (int)(2000 * 1.0));


int main()
{
    game.Create();

    while(game.Update()) {}
    
    game.Destroy();

    return 0;
}
