#include "ship.h"



Ship::Ship()
{
    nScrap = 0;
    nFood = 20;
}



Ship::~Ship()
{
}



void Ship::Update(Event &cEvent)
{
    nFood -= cEvent.mCost["food"];
}
