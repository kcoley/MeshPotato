#CC=g++
CC=clang++
CFLAGS=-c -Wall -fPIC

INCLUDES= -I ./include -I $(VDBINCLUDE)
LINKS= -L$(VDBLIB) -lopenvdb
PLUGINFILES=$(*.C)
PLUGINNAME=libOBJOutputPlugin.so


all: $(PLUGINFILES)
	$(CC) -fPIC -c -I ../../include OBJOutputPlugin.C -o mpobjoutputplugin.o
	$(CC) -shared -Wl,-soname,$(PLUGINNAME) -o $(PLUGINNAME) mpobjoutputplugin.o

clean:
	rm *.o
	rm *.so
