#!/bin/sh
g++ testVectors.C -I ../../include -L ../../lib -lmeshpotato -lopenvdb -o testVectors -ldl
