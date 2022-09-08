#ifndef SYNC_WAIT_SERVER_H
#define SYNC_WAIT_SERVER_H

#include "server.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <openssl/md5.h>

class SyncWaitServer: public Server {
public:
    SyncWaitServer() {
        MD5_Init(&_md5_ctx);
    }

    virtual void recv_request() override;
protected:
    MD5_CTX _md5_ctx;
};

#endif
