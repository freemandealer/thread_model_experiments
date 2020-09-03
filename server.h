#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <ctime>
#include <pthread.h>
#include <assert.h>

using namespace std;

#define BUF_SIZE (1024*1024*4)

class Server {
public:
    virtual void handle_request(char *buf) {
        //cout << "fake handle" << endl;
        sleep(3);
    }
};

#endif
