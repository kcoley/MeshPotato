CC = g++
CFLAGS = -g 
VDB_INC = /group/dpa/include/openvdb/include
VDB_LIB = /group/dpa/local/openvdb/lib
OPENEXR_INC = /group/dpa/include/OpenEXR
OIIO_INC= /group/dpa/include
OIIO_LIB= /group/dpa/lib
MESHPOTATO_LIB = $(HOME)/projects/MeshPotato/lib
MESHPOTATO_INC = $(HOME)/projects/MeshPotato/include
FLAGS = -lOpenImageIO -lmeshpotato -lImath -lboost_thread -lIex -lpthread -lz -lHalf -lopenvdb -lboost_system -lboost_filesystem -lboost_thread -ltbb -lfftw3
EXECUTABLE = volumeTest
SOURCE = rayMarchTest.C
INCLUDES = -I $(VDB_INC) -I $(OPENEXR_INC) -I $(MESHPOTATO_INC) -I $(OIIO_INC)
LIBS = -L$(VDB_LIB) -L$(MESHPOTATO_LIB) -L$(OIIO_LIB)

all: $(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) $(INCLUDES) $(LIBS) $(FLAGS) -o $(EXECUTABLE)

