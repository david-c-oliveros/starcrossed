#include "ship.h"



Ship::Ship()
{
    vecDebugInfo.push_back("Ship");
    for (int32_t i = 0; i < 8; i++)
        vecDebugInfo.push_back("");

    nScrap = 0;
    nFood = 200;
}



Ship::~Ship()
{
}



void Ship::EventUpdate(Event &cEvent)
{
    nFood -= cEvent.mCost["food"];
}
