#pragma once



class Ship
{
    public:
        Ship();
        ~Ship();
        Update(Event &cEvent);


    public:
        int32_t nFood;
        int32_t nScrap;
};
