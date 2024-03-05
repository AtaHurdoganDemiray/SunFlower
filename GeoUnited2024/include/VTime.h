// VTime.h
#ifndef VTIME_H
#define VTIME_H

#include <ctime>

class VTime
{
public:
    VTime();
    VTime(const std::tm& timeStruct);

    static VTime GetCurrentTime();
    
    int GetYear() const;
    int GetMonth() const;
    int GetDay() const;
    int GetHour() const;
    int GetMinute() const;
    int GetSecond() const;

private:
    std::tm timeStruct;
};

#endif // VTIME_H
