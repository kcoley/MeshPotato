CC=g++
CFLAGS=-c -Wall -fPIC
LDFLAGS=
SOURCES=$(wildcard src/*.C)
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib
OIIO_INC= /group/dpa/include
OIIO_LIB= /group/dpa/lib

PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)

PYTHON_TARGET = lib/meshpotato_python
OBJECTS=$(SOURCES:.C=.o)
PLUGINS=plugins
VDBINCLUDE=/group/dpa/local/include
VDBLIB=/group/dpa/local/openvdb/lib
INCLUDES= -I ./include -I $(VDBINCLUDE) -I $(BOOST_INC) -I $(OIIO_INC) -I /group/dpa/include 
LINKS= -L$(VDBLIB) -lopenvdb -L$(BOOST_LIB) -L$(OIIO_LIB) -lboost_filesystem -L/group/dpa/lib -lOpenImageIO 
MESHCONVERTERLIB=lib/libmeshpotato.a
MESHPOTATOSHAREDLIBNAME=libmeshpotato.so
MESHPOTATOSHAREDLIB=lib/libmeshpotato.so
PLUGINFILES=plugins/*/*.C
PYTARGET=pymeshpotato/pymeshpotato
PYMPVOLUME=python/mpvolume
PYMPMESH=python/mpmesh
LIB_MAJOR_VERSION=$(shell grep 'define MESHPOTATO_API_MAJOR ' include/MPPlugins/version.h | sed 's/[^0-9]*//g')
LIB_MINOR_VERSION=$(shell grep 'define MESHPOTATO_API_MINOR ' include/MPPlugins/version.h | sed 's/[^0-9]*//g')
LIB_PATCH_VERSION=$(shell grep 'define MESHPOTATO_API_PATCH ' include/MPPlugins/version.h | sed 's/[^0-9]*//g')
LIB_VERSION=$(LIB_MAJOR_VERSION).$(LIB_MINOR_VERSION).$(LIB_PATCH_VERSION)
SO_VERSION=$(LIB_MAJOR_VERSION).$(LIB_MINOR_VERSION)

%.o: %.C
	$(CC) $(INCLUDES) -o $@ $< $(CFLAGS) 


$(MESHPOTATOSHAREDLIB).$(SO_VERSION): $(OBJECTS)
	$(CC) -shared -Wl,-soname,libmeshpotato.so.$(SO_VERSION) -o $(MESHPOTATOSHAREDLIB).$(SO_VERSION)  $(OBJECTS) $(LINKS)
	ln -f -s $(MESHPOTATOSHAREDLIBNAME).$(SO_VERSION) $(MESHPOTATOSHAREDLIBNAME)
	mv $(MESHPOTATOSHAREDLIBNAME) $(MESHPOTATOSHAREDLIB)
pympmesh: $(OBJECTS)
	$(CC) $(CFLAGS) -I $(PYTHON_INCLUDE) $(PYMPMESH).C -o $(PYMPMESH).o
	$(CC) -shared -Wl,-soname,mpmesh.so $(PYMPMESH).o -lpython2.7 -lboost_python -lboost_thread -lboost_system -lboost_filesystem -L./lib -L/group/dpa/local/openvdb/lib -lmeshpotato -lopenvdb /group/dpa/local/openvdb/python/lib/python2.7/pyopenvdb.so -lboost_system -o pymeshpotato/mpmesh.so 

pympvolume: $(OBJECTS)
	$(CC) $(CFLAGS) -I$(PYTHON_INCLUDE) $(PYMPVOLUME).C -o $(PYMPVOLUME).o
	$(CC) -shared -Wl,-soname,mpvolume.so $(PYMPVOLUME).o -lpython2.7 -lboost_python -lboost_thread -lboost_system -lboost_filesystem -L./lib -L/group/dpa/local/openvdb/lib -lmeshpotato -lopenvdb /group/dpa/local/openvdb/python/lib/python2.7/pyopenvdb.so -lboost_system -o pymeshpotato/mpvolume.so 

python: pympmesh pympvolume
clean:
	rm src/*.o
	rm lib/*.so*
	rm python/*.o

doc:
	doxygen Doxyfile
plugins:
	cd ./plugins/OBJInput; make
	cd ./plugins/OBJOutput; make
cleanplugins:
	cd ./plugins/OBJInput; make
	rm plugins/*/*.so
