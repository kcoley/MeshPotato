#CC=g++
CC=clang++
CFLAGS=-c -Wall -fPIC

INCLUDES= -I ./include -I $(VDBINCLUDE)

LINKS= -L$(VDBLIB) -lopenvdb
PLUGINFILES=$(*.C)
PLUGINNAME=libVDBOutputPlugin.so


all: $(PLUGINFILES)
	$(CC) $(CFLAGS) -I ../../include  VDBOutputPlugin.C -o mpvdboutputplugin.o
	$(CC) -shared -Wl,-soname,$(PLUGINNAME) -o $(PLUGINNAME) mpvdboutputplugin.o

clean:
	rm *.o
	rm *.so
