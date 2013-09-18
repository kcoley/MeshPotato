#!/bin/sh
g++ MyApplication.C -I ../../include -L ../../lib -lmeshpotato -lopenvdb -o MyApplication -ldl
