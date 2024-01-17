#include "ship.h"



Ship::Ship()
{
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
