#include "app.h"


App gameApp((int)(1920 * 1.0), (int)(1080 * 1.0));


int main(int argc, const char* argv[])
{
    gameApp.Create(argv[1]);

    while(gameApp.Update()) {}
    
    gameApp.Destroy();

    return 0;
}
