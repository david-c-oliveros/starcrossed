#include "timer.h"



Timer::Timer(uint32_t _nTotalTicks)
    : m_nTotalTicks(_nTotalTicks), m_bRunning(false), m_nCurTick(0)
{
}



Timer::~Timer()
{
}



void Timer::Start()
{
    m_bRunning = true;
}



void Timer::Reset()
{
    m_bRunning = false;
    m_nCurTick = 0;
}



void Timer::SetTime(uint32_t _nTotalTicks)
{
    m_nTotalTicks = _nTotalTicks;
}



void Timer::Update()
{
    if (m_bRunning)
        m_nCurTick++;
}



bool Timer::Check()
{
    return m_nCurTick >= m_nTotalTicks;
}
