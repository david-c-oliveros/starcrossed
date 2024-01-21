#include "app.h"


App gameApp((int)(1920 * 1.0), (int)(1080 * 1.0));


int main()
{
    gameApp.Create();

    while(gameApp.Update()) {}
    
    gameApp.Destroy();

    return 0;
}
