#CC=g++
CC=g++
CFLAGS=-Wall -g
VDBLIB=/usr/local/lib
MESHPOTATOLIB=$(HOME)/dev/MeshPotato/install/lib -L/group/dpa/local/Alembic/lib/static
HDF5_INC = -I/usr/local/include
HDF5_LIB = -L/usr/local/lib
HDF5_FLAGS = -lhdf5_hl -lhdf5
INCLUDES= -I ../../include -I /usr/local/include -I /usr/include/OpenEXR $(HDF5_INC) $(HDF5_FLAGS)
LINKS= -L/usr/local/lib -L$(VDBLIB) -lopenvdb -lboost_system -lboost_filesystem -L$(MESHPOTATOLIB) -lmeshpotato
PLUGINFILES=$(*.C) 
PLUGINNAME=mpconvert


all: $(PLUGINFILES)
	$(CC) $(CFLAGS) $(PLUGINFILES) mpconvert.C $(INCLUDES) $(LINKS) -o mpconvert 
	cp $(PLUGINNAME) ../../bin
clean:
	rm *.o
