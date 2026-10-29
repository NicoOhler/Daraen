#pragma once

#include "definitions.h"
#include "utils.h"
#include "Logger.h"
#include <string>
#include <iostream>
#include <chrono>

inline Milliseconds getMilliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now().time_since_epoch())
        .count();
}

class Timer
{
public:
    void start()
    {
        start_time = getMilliseconds();
    }

    Milliseconds stop(LogType log_type = TIME)
    {
        end_time = getMilliseconds();
        log(log_type, "Time taken: " + convertMillisecondsToTimeString(end_time - start_time));
        return end_time;
    }

    Milliseconds getElapsedTime()
    {
        return getMilliseconds() - start_time;
    }

private:
    Milliseconds start_time;
    Milliseconds end_time;
};
