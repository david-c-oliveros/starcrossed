#pragma once

#include "event.h"



class Ship
{
    public:
        Ship();
        ~Ship();
        void Update(Event &cEvent);


    public:
        int32_t nFood;
        int32_t nScrap;
};
