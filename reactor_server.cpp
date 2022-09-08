#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include "reactor_server.h"
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <cstring>

using namespace std;

ReactorServer::ReactorServer() {
    auto thread_loop = [=] {
        while (true) {
            if (_task_queue.empty())
                continue;
            auto f = _task_queue.front();
            _task_queue.pop();
            f();
    }
    };
    _t = new thread(thread_loop);
}

ReactorServer::~ReactorServer() {
    _t->join();
}

void ReactorServer::enque_task() {
    _task_queue.push(
        [=] {
            int fd = open("/dev/vda", 0);
            int ret = 0;
            char *buf = (char *)malloc(BUF_SIZE);
            ret = read(fd, buf, BUF_SIZE);
            assert(ret == BUF_SIZE);

            //memset(buf, 1, BUF_SIZE);
            unsigned char md5buf[MD5_DIGEST_LENGTH];
            MD5_Update(&_md5_ctx, (unsigned char *)buf, BUF_SIZE/10);
            MD5_Final(md5buf, &_md5_ctx);
            g_flying_ops.fetch_sub(1, std::memory_order_relaxed);
            g_total_ops.fetch_add(1, std::memory_order_relaxed);
            free(buf);
        }
    );
}

void ReactorServer::recv_request() {
    enque_task();
}

