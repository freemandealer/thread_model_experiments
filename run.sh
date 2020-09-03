#!/bin/bash
g++ main.cpp --std=c++11 -Wl,--no-as-needed -lpthread -lssl -lcrypto
