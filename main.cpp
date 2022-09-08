#include <iostream>
#include <thread>
#include <atomic>
#include <time.h>
#include <csignal>
#include <unistd.h>
#include <stdlib.h>
#include "sync_wait_server.h"
#include "sync_wait_multi_thread_server.h"
#include "reactor_server.h"
#include "reactor_aio_server.h"
#include "server.h"
#include <chrono>
#include <ctime>
#include <pthread.h>
#include <assert.h>

#define CLIENT_THREAD_NUM 1

using namespace std;

Server *g_server;
bool g_stop;
atomic<uint64_t> g_total_ops;
atomic<uint64_t> g_flying_ops;

void signalHandler( int signum )
{
    cout << "stopping" << endl;
    g_stop = true;
}

void send_request() {
    g_server->recv_request();
}

void thread_function() {
    cout << "thread started" << endl;
    g_flying_ops = 0;

    while (!g_stop) {
        if (g_flying_ops >= NUM_FLYING ) {
            continue;
        }
        send_request();
        g_flying_ops.fetch_add(1, std::memory_order_relaxed);
    }
}

int main(int argc, char **argv) {
    srand(time(NULL));
    cout << "pid: " << getpid() << endl;
    g_stop = false;
    g_total_ops = 0;
    g_flying_ops = 0;

    int which = atoi(argv[1]);
    switch(which) {
    case 0: g_server = new SyncWaitServer(); break;
    case 1: g_server = new SyncWaitMultiThreadServer(); break;
    case 2: g_server = new ReactorServer(); break;
    case 3: g_server = new ReactorAIOServer(); break;
    }

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

    printf("tp:\t%.3f op/s\n", g_total_ops/elapsed_seconds.count());

    return 0;
}
