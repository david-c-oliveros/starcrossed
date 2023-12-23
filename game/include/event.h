#pragma once



class Event
{
    public:
        Event();
        ~Event();


    public:
        std::map<std::string, int32_t> mCost;
};
