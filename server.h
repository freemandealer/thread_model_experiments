#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <ctime>
#include <pthread.h>
#include <assert.h>
#include <atomic>

using namespace std;

#define BUF_SIZE (1024*1024*16)
#define NUM_FLYING 10

extern atomic<uint64_t> g_total_ops;
extern atomic<uint64_t> g_flying_ops;

class Server {
public:
    virtual void recv_request();
};

#endif
