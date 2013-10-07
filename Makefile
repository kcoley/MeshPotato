#CC=g++
CC=clang++
CFLAGS=-c -Wall -fPIC
LDFLAGS=
SOURCES=$(wildcard src/*.C)
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib

PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)


PYTHON_TARGET = lib/meshpotato_python
#SOURCES=src/MPMesh.C \
src/Vector.C \
src/MeshPotato.C \
src/MeshPotatoPlugin.C \
src/MeshPotatoPluginManager.C \
src/MyEngine.C \
src/Plugin.C
OBJECTS=$(SOURCES:.C=.o)
EXECUTABLE=transformer
PLUGINS=plugins
#Paths for OpenVDB
VDBINCLUDE=${HOME}/projects/OpenVDB/include
VDBLIB=${HOME}/projects/OpenVDB/lib

INCLUDES= -I ./include -I $(VDBINCLUDE) -I $(BOOST_INC)
LINKS= -L$(VDBLIB) -lopenvdb -L$(BOOST_LIB) -lboost_filesystem
MESHCONVERTERLIB=lib/libmeshpotato.a
MESHPOTATOSHAREDLIB=lib/libmeshpotato.so
PLUGINFILES=plugins/*/*.C
%.o: %.C
	$(CC) $(INCLUDES) $(LINKS) -o $@ $< $(CFLAGS) 

#$(MESHCONVERTERLIB): $(OBJECTS)
#	ar rcs $(MESHCONVERTERLIB) $(OBJECTS) 

$(MESHPOTATOSHAREDLIB): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS)
	$(CC) -shared -Wl,-soname,libmeshpotato.so -o $(MESHPOTATOSHAREDLIB) $(OBJECTS)

python: $(OBJECTS)
	g++ -shared -Wl,--export-dynamic $(OBJECTS) $(LINKS) -lboost_python -L/usr/lib/python$(PYTHON_VERSION)/config -lboost_python -o $(PYTHON_TARGET).so

clean:
	rm src/*.o
	rm lib/*.so

doc:
	doxygen Doxyfile
plugins:
	cd ./plugins/OBJInput; make
	cd ./plugins/OBJOutput; make
cleanplugins:
	cd ./plugins/OBJInput; make
	rm plugins/*/*.so
