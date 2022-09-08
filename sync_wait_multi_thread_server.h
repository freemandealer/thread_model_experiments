#ifndef SYNC_WAIT_MULTI_THREAD_SERVER_H
#define SYNC_WAIT_MULTI_THREAD_SERVER_H

#include "sync_wait_server.h"
#include <vector>
#include <thread>

using namespace std;

class SyncWaitMultiThreadServer: public SyncWaitServer {
public:
    ~SyncWaitMultiThreadServer() {
        for (auto itr = _t.begin(); itr != _t.end(); ++itr) {
            (*itr)->join();
        }
    }
    void recv_request() override;
private:
    vector<thread*> _t;
};

#endif
