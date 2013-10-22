#CC=g++
CC=clang++
CFLAGS=-c -Wall -fPIC

INCLUDES= -I ../../include
LINKS= -L$(VDBLIB) -lopenvdb
PLUGINFILES=$(*.C)
PLUGINNAME=libVDBInputPlugin.so


all: $(PLUGINFILES)
	$(CC) -fPIC -c $(INCLUDES) VDBInputPlugin.C -o mpvdbinputplugin.o
	$(CC) -shared -Wl,-soname,$(PLUGINNAME) -o $(PLUGINNAME) mpvdbinputplugin.o $(LINKS)

clean:
	rm *.o
	rm *.so
