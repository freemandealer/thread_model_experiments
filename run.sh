#!/bin/bash
g++ main.cpp server.cpp sync_wait_server.cpp sync_wait_multi_thread_server.cpp reactor_server.cpp reactor_aio_server.cpp ../libaio/src/libaio.a  --std=c++11 -Wl,--no-as-needed -lpthread -lssl -lcrypto -lpthread
