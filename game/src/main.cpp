#include "app.h"


//Game game((int)(1920 * 1.0), (int)(1080 * 1.0));
App gameApp((int)(2000 * 1.0), (int)(2000 * 1.0));


int main()
{
    gameApp.Create();

    while(gameApp.Update()) {}
    
    gameApp.Destroy();

    return 0;
}
