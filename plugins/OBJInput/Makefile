#CC=g++
CC=clang++
CFLAGS=-c -Wall -fPIC

INCLUDES= -I ./include -I $(VDBINCLUDE)
LINKS= -L$(VDBLIB) -lopenvdb
PLUGINFILES=$(*.C)
PLUGINNAME=libOBJInputPlugin.so


all: $(PLUGINFILES)
	$(CC) -fPIC -c -I ../../include OBJInputPlugin.C -o mpobjinputplugin.o
	$(CC) -shared -Wl,-soname,$(PLUGINNAME) -o $(PLUGINNAME) mpobjinputplugin.o

clean:
	rm *.o
	rm *.so
