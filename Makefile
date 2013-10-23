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
VDBINCLUDE=${HOME}/projects/OpenVDB/include
VDBLIB=${HOME}/projects/OpenVDB/lib
ALEMBICINCLUDE=/home/kcoley/Downloads/Alembic_150/Alembic_1.5.0_2013072300/lib
ALEMBICLIB=-Wl,-E -L/home/kcoley/projects/alembic/lib -L/home/kcoley/projects/alembic/lib/Alembic/Abc/ -L/home/kcoley/projects/alembic/lib/Alembic/AbcGeom -L/home/kcoley/projects/alembic/lib/Alembic/AbcCoreHDF5 -L/home/kcoley/projects/alembic/lib/Alembic/AbcCollection -L/home/kcoley/projects/alembic/lib/Alembic/AbcMaterial -L/home/kcoley/projects/alembic/lib/AbcOpenGL -L/home/kcoley/projects/alembic/lib/Alembic/AbcCoreAbstract -L/home/kcoley/projects/alembic/lib/Alembic/Util -lAlembicAbc -lAlembicAbcGeom -lAlembicAbcCoreHDF5 -lAlembicAbcCollection -lAlembicAbcMaterial -lAlembicAbcOpenGL -lImath -lIex -lboost_thread -lhdf5_hl -lhdf5 -lpthread -lz -lAlembicAbcCoreAbstract -lAlembicUtil -lHalf
INCLUDES= -I ./include -I $(VDBINCLUDE) -I $(BOOST_INC) 
LINKS= -L$(VDBLIB) -lopenvdb -L$(BOOST_LIB) -lboost_filesystem -L$(HOME)/projects/usr/local/lib -lhdf5
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
