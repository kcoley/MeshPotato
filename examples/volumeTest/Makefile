CC = g++
CFLAGS = -g
VDB_INC = /group/dpa/include/openvdb/include
VDB_LIB = /group/dpa/local/openvdb/lib
OPENEXR_INC = /usr/include/OpenEXR
MESHPOTATO_LIB = $(HOME)/projects/MeshPotato/lib
MESHPOTATO_INC = $(HOME)/projects/MeshPotato/include
FLAGS = -lmeshpotato -lImath -lboost_thread -lIex -lpthread -lz -lHalf -lopenvdb -lboost_system -lboost_filesystem -lboost_thread -ltbb
EXECUTABLE = volumeTest
SOURCE = volumeTest.C
INCLUDES = -I $(VDB_INC) -I $(OPENEXR_INC) -I $(MESHPOTATO_INC)
LIBS = -L$(VDB_LIB) -L$(MESHPOTATO_LIB)

all: $(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) $(INCLUDES) $(LIBS) $(FLAGS) -o $(EXECUTABLE)

#g++ -g volumeTest.C -I ../../include -I /usr/include/OpenEXR -I /group/dpa/include/openvdb/include -L$HOME/projects/MeshPotato/lib -lmeshpotato -L/group/dpa/local/openvdb/lib -lImath -lboost_thread -lIex -lpthread -lz -lHalf -lopenvdb -lboost_system -lboost_filesystem -lboost_thread -ltbb -o volumeTest
