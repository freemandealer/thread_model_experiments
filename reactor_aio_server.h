#ifndef REACTOR_AIO_SERVER_H
#define REACTOR_AIO_SERVER_H

#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include <functional>
#include <queue>
#include <functional>
#include "sync_wait_server.h"
#include <thread>

class ReactorAIOServer: public SyncWaitServer {
public:
    ReactorAIOServer();
    ~ReactorAIOServer();
    void recv_request();
    void enque_task();
private:
    queue<function<void()>> _task_queue;
    thread *_t;
    thread *_completion_t;
};

#endif
