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

    void handle_request(char *buf) override {
        int fd = open("/dev/urandom", 0);
        int ret = 0;
        ret = read(fd, buf, BUF_SIZE);
        assert(ret == BUF_SIZE);

        unsigned char md5buf[MD5_DIGEST_LENGTH];
        MD5_Update(&_md5_ctx, (unsigned char *)buf, BUF_SIZE);
        MD5_Final(md5buf, &_md5_ctx);
    }
private:
    MD5_CTX _md5_ctx;
};

