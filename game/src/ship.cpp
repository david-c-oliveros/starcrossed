#include "ship.h"



Ship::Ship()
{
}



Ship::~Ship()
{
}



Ship::Update(Event &cEvent)
{
    nFood -= cEvent.mCost["food"];
}
