// VTime.cpp
#include "StdAfx.h"
#include "VTime.h"


VTime::VTime()
{
    time_t now = std::time(0);
    timeStruct = *std::localtime(&now);
}

VTime::VTime(const std::tm& timeStruct)
    : timeStruct(timeStruct)
{
}

VTime VTime::GetCurrentTime()
{
    // Get current time
    time_t now = std::time(0);
    return VTime(*std::localtime(&now));
}

int VTime::GetYear() const
{
    return timeStruct.tm_year + 1900; 
}

int VTime::GetMonth() const
{
    return timeStruct.tm_mon + 1; 
}

int VTime::GetDay() const
{
    return timeStruct.tm_mday;
}

int VTime::GetHour() const
{
    return timeStruct.tm_hour;
}

int VTime::GetMinute() const
{
    return timeStruct.tm_min;
}

int VTime::GetSecond() const
{
    return timeStruct.tm_sec;
}

