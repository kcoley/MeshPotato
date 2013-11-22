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
OBJECTS=$(SOURCES:.C=.o)
PLUGINS=plugins
#Paths for OpenVDB
VDBINCLUDE=/group/dpa/local/include
VDBLIB=/group/dpa/local/openvdb/lib
INCLUDES= -I ./include -I $(VDBINCLUDE) -I $(BOOST_INC) 
LINKS= -L$(VDBLIB) -lopenvdb -L$(BOOST_LIB) -lboost_filesystem 
MESHCONVERTERLIB=lib/libmeshpotato.a
MESHPOTATOSHAREDLIB=lib/libmeshpotato.so
PLUGINFILES=plugins/*/*.C
%.o: %.C
	$(CC) $(INCLUDES) -o $@ $< $(CFLAGS) 
#	$(CC) $(INCLUDES) $(LINKS) -o $@ $< $(CFLAGS) 

#$(MESHCONVERTERLIB): $(OBJECTS)
#	ar rcs $(MESHCONVERTERLIB) $(OBJECTS) 

$(MESHPOTATOSHAREDLIB): $(OBJECTS)
#	$(CC) $(CFLAGS) $(SOURCES)
	$(CC) -shared -Wl,-soname,libmeshpotato.so -o $(MESHPOTATOSHAREDLIB) $(OBJECTS) $(LINKS)

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
