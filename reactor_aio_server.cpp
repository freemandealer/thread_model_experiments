#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>
#include "reactor_aio_server.h"
#include <stdlib.h>
#include <unistd.h>
#include <atomic>
#include <cstring>
#include <libaio.h>
#include <errno.h>

using namespace std;

io_context_t ctx;

ReactorAIOServer::ReactorAIOServer() {
    memset(&ctx, 0, sizeof(ctx));
    io_queue_init(10, &ctx); //TODO: arbitary 100
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
    //auto completion_loop = [=] {
    //    int ret = 0;
    //    while (1) {
    //        io_queue_run(ctx);
    //    }

    //};
    //_completion_t = new thread(completion_loop);
}

ReactorAIOServer::~ReactorAIOServer() {
    _t->join();
    _completion_t->join();
}

MD5_CTX g_md5_ctx;

void rd_done(io_context* ctx, iocb* iocb, long int res, long int res2) {
    /* library needs accessors to look at iocb? */
    int iosize = iocb->u.c.nbytes;
    char *buf = reinterpret_cast<char*>(iocb->u.c.buf);
    off_t offset = iocb->u.c.offset;

    cout << "read done done: " << iosize << endl;
    if (res2 != 0)
        cout << "aio read error: " << res2;
    if (res != iosize) {
        fprintf(stderr, "read missing bytes expect %d got %d\n", iocb->u.c.nbytes, res);
        exit(1);
    }

    unsigned char md5buf[MD5_DIGEST_LENGTH];
    MD5_Update(&g_md5_ctx, (unsigned char *)buf, BUF_SIZE/10);
    MD5_Final(md5buf, &g_md5_ctx);
    g_flying_ops.fetch_sub(1, std::memory_order_relaxed);
    g_total_ops.fetch_add(1, std::memory_order_relaxed);
    free(buf);
}

void ReactorAIOServer::enque_task() {
    _task_queue.push(
        [=] {
            cout << "coming new" << endl;
            struct iocb cb;
            struct iocb *iocbs = &cb;
            struct io_event events[1];

            int fd = open("/dev/vda", O_RDONLY | O_DIRECT, 0644); // must be O_DIRECT, other wise fall back to sync I/O
            int ret = 0;
            int pagesize = sysconf(_SC_PAGESIZE);
            char *buf = NULL;
            posix_memalign(reinterpret_cast<void**>(&buf), pagesize, BUF_SIZE);

            memset(buf, 0, BUF_SIZE);
            memset(&cb, 0, sizeof(cb));


            //if (io_setup(1, &ctx) < 0) {
            //    cout << "error io_setup" << endl;
            //    exit(-1);
            //}
            io_prep_pread(&cb, fd, buf, BUF_SIZE, 0);
            io_set_callback(&cb, rd_done);
            ret = io_submit(ctx, 1, &iocbs);
            if (ret < 0) {
                cout << "error io_submit: " << ret << endl;
                exit(-2);
            }
            //io_queue_wait(ctx, NULL);
            ret = io_queue_run(ctx);



#if 0

#endif
        }
    );
}

void ReactorAIOServer::recv_request() {
    enque_task();
}

