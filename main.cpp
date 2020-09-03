#include <iostream>
#include <thread>
#include <atomic>
#include <time.h>
#include <csignal>
#include <unistd.h>
#include <stdlib.h>
#include "sync_wait_server.h"
#include <chrono>
#include <ctime>
#include <pthread.h>
#include <assert.h>

#define CLIENT_THREAD_NUM 10

using namespace std;

Server *g_server;
bool g_stop;
atomic<uint64_t> g_num_ops;

void signalHandler( int signum )
{
    cout << "stopping" << endl;
    g_stop = true;
}

void thread_function() {
    uint64_t num_ops = 0;
    cout << "thread started" << endl;
    char *buf = nullptr;
    buf = (char *)malloc(BUF_SIZE);
    assert(buf != nullptr);
    while (!g_stop) {
        g_server->handle_request(buf);
        ++ num_ops;
    }
    // cout << num_ops << endl;
    g_num_ops.fetch_add(num_ops, std::memory_order_relaxed);
}

int main() {
    srand(time(NULL));
    g_server = new SyncWaitServer();
    g_stop = false;
    signal(SIGINT, signalHandler);
    thread *t[CLIENT_THREAD_NUM];

    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < CLIENT_THREAD_NUM; ++i) {
        t[i] = new thread(thread_function);
    }

    for (int i = 0; i < CLIENT_THREAD_NUM; ++i)
        t[i]->join();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    cout << elapsed_seconds.count() << endl;

    printf("tp:\t%.3f op/s\n", g_num_ops/elapsed_seconds.count());

    return 0;
}
