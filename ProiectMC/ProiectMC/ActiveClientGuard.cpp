#include "ActiveClientGuard.h"

ActiveClientGuard::ActiveClientGuard(std::atomic<int>& clients) : active_clients(clients) {
    ++active_clients;
}

ActiveClientGuard::~ActiveClientGuard() {
    --active_clients;
}
