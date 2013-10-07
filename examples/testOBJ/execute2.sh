#!/bin/sh
g++ readingAndWriting.C -I ../../include -I ../../plugins/VDBOutput/ -L ../../lib -lmeshpotato -L ../../../OpenVDB/lib -lopenvdb -lboost_system -lboost_filesystem -o readingAndWriting -ldl
