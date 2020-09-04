#include <iostream>
#include <ctime>
#include <pthread.h>
#include <assert.h>
#include "server.h"
#include <atomic>
#include <unistd.h>

void Server::recv_request() {
    sleep(2);
    g_flying_ops.fetch_sub(1, std::memory_order_relaxed);
    g_total_ops.fetch_add(1, std::memory_order_relaxed);
}

