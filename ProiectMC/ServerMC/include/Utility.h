#pragma once
#include <chrono>

inline int getCurrentTimeInSeconds() {
    using namespace std::chrono;
    return static_cast<int>(duration_cast<seconds>(
        system_clock::now().time_since_epoch()).count());
}