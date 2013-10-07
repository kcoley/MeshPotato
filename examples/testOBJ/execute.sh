#!/bin/sh
g++ MyApplication.C -I ../../include -L ../../lib -lmeshpotato -lopenvdb -lboost_system -lboost_filesystem -o MyApplication -ldl
