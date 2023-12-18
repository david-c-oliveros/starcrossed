#pragma once


#include "engine_defines.h"


class Timer
{
    public:
        Timer(uint32_t _nTotalTicks = 0);
        ~Timer();

        void Start();
        void Reset();
        void SetTime(uint32_t _nTotalTicks);
        void Update();
        bool Check();

    private:
        uint32_t m_nCurTick;
        uint32_t m_nTotalTicks;
        bool m_bRunning;
};
