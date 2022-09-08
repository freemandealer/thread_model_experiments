#ifndef REACTOR_SERVER_H
#define REACTOR_SERVER_H

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

class ReactorServer: public SyncWaitServer {
public:
    ReactorServer();
    ~ReactorServer();
    void recv_request() override;
    void enque_task();
private:
    queue<function<void()>> _task_queue;
    thread *_t;
};

#endif
