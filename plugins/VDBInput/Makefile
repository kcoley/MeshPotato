#CC=g++
CC=clang++
CFLAGS=-c -Wall -fPIC
VDBINCLUDE= /group/dpa/local/include
VDBLIB= /group/dpa/local/openvdb/lib
INCLUDES= -I ../../include -I $(VDBINCLUDE)
LINKS= -L$(VDBLIB) -lopenvdb 
PLUGINFILES=$(*.C)
PLUGINNAME=libVDBInputPlugin.so


all: $(PLUGINFILES)
	$(CC) $(CFLAGS) $(INCLUDES) VDBInputPlugin.C -o mpvdbinputplugin.o
	$(CC) -shared -Wl,-soname,$(PLUGINNAME) -o $(PLUGINNAME) mpvdbinputplugin.o $(LINKS)

clean:
	rm *.o
	rm *.so
