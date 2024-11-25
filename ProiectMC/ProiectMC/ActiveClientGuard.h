#pragma once
#include <atomic>

class ActiveClientGuard {
public:

    ActiveClientGuard(std::atomic<int>& clients);

    ~ActiveClientGuard();

private:
    std::atomic<int>& active_clients;
};
