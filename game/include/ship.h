#pragma once

#include "event.h"



class Ship
{
    public:
        Ship();
        ~Ship();
        void EventUpdate(Event &cEvent);


    public:
        int32_t nFood;
        int32_t nScrap;
};
