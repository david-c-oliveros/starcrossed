#pragma once

#include <vector>

#include "event.h"



class Ship
{
    public:
        Ship();
        ~Ship();
        void EventUpdate(Event &cEvent);


    public:
        std::vector<std::string> vecDebugInfo;

        int32_t nFood;
        int32_t nScrap;
};
