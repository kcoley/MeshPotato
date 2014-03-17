#CC=g++
CC=g++
CFLAGS=-Wall -g
VDBLIB=/group/dpa/local/openvdb/lib
MESHPOTATOLIB=$(HOME)/projects/MeshPotato/lib -L/group/dpa/local/Alembic/lib/static
HDF5_INC = -I/group/dpa/local/hdf5/include
HDF5_LIB = -L/group/dpa/local/hdf5/lib
HDF5_FLAGS = -lhdf5_hl -lhdf5
INCLUDES= -I ../../include -I /usr/include/OpenEXR $(HDF5_INC) $(HDF5_FLAGS)
LINKS= -L$(VDBLIB) -lopenvdb -lboost_system -lboost_filesystem -L$(MESHPOTATOLIB) -lmeshpotato
PLUGINFILES=$(*.C) 
PLUGINNAME=mpconvert


all: $(PLUGINFILES)
	$(CC) $(CFLAGS) $(PLUGINFILES) mpconvert.C $(INCLUDES) $(LINKS) -o mpconvert 
	cp $(PLUGINNAME) ../../bin
clean:
	rm *.o
