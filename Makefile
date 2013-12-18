CC=clang++
CFLAGS=-c -Wall -fPIC
LDFLAGS=
SOURCES=$(wildcard src/*.C)
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)

PYTHON_TARGET = lib/meshpotato_python
OBJECTS=$(SOURCES:.C=.o)
PLUGINS=plugins
VDBINCLUDE=/group/dpa/local/include
VDBLIB=/group/dpa/local/openvdb/lib
INCLUDES= -I ./include -I $(VDBINCLUDE) -I $(BOOST_INC) 
LINKS= -L$(VDBLIB) -lopenvdb -L$(BOOST_LIB) -lboost_filesystem 
MESHCONVERTERLIB=lib/libmeshpotato.a
MESHPOTATOSHAREDLIB=lib/libmeshpotato.so
PLUGINFILES=plugins/*/*.C
PYTARGET=pymeshpotato/pymeshpotato
PYMPVOLUME=python/mpvolume
PYMPMESH=python/mpmesh
%.o: %.C
	$(CC) $(INCLUDES) -o $@ $< $(CFLAGS) 


$(MESHPOTATOSHAREDLIB): $(OBJECTS)
	$(CC) -shared -Wl,-soname,libmeshpotato.so -o $(MESHPOTATOSHAREDLIB) $(OBJECTS) $(LINKS)

pympmesh: $(OBJECTS)
	$(CC) $(CFLAGS) -I $(PYTHON_INCLUDE) $(PYMPMESH).C -o $(PYMPMESH).o
	$(CC) -shared -Wl,-soname,mpmesh.so $(PYMPMESH).o -lpython2.7 -lboost_python -lboost_thread -lboost_system -lboost_filesystem -L./lib -L/group/dpa/local/openvdb/lib -lmeshpotato -lopenvdb /group/dpa/local/openvdb/python/lib/python2.7/pyopenvdb.so -lboost_system -o pymeshpotato/mpmesh.so

pympvolume: $(OBJECTS)
	$(CC) $(CFLAGS) -I$(PYTHON_INCLUDE) -c $(PYMPVOLUME).C -o $(PYMPVOLUME).o
	$(CC) -shared -Wl,-soname,mpmesh.so -o pymeshpotato/mpvolume.so $(PYMPVOLUME).o -lpython2.7 -lboost_python -L./lib -lmeshpotato 
clean:
	rm src/*.o
	rm python/*.o
	rm lib/*.so

doc:
	doxygen Doxyfile
plugins:
	cd ./plugins/OBJInput; make
	cd ./plugins/OBJOutput; make
cleanplugins:
	cd ./plugins/OBJInput; make
	rm plugins/*/*.so
