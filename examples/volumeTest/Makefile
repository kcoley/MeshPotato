CC = g++
CFLAGS = -g
VDB_INC = /group/dpa/include/openvdb/include
#VDB_INC = /home/kcoley/projects/OpenVDB/include
VDB_LIB = /group/dpa/local/openvdb/lib
#VDB_LIB = /home/kcoley/projects/OpenVDB/lib
OPENEXR_INC = /usr/include/OpenEXR
OIIO_INC= /group/dpa/local/oiio/include
OIIO_LIB= /group/dpa/local/oiio/lib
MESHPOTATO_LIB = $(HOME)/projects/MeshPotato/lib
MESHPOTATO_INC = $(HOME)/projects/MeshPotato/include
FLAGS = -lOpenImageIO -lmeshpotato -lImath -lboost_thread -lIex -lpthread -lz -lHalf -lopenvdb -lboost_system -lboost_filesystem -lboost_thread -ltbb
EXECUTABLE = volumeTest
SOURCE = rayMarchTest.C
INCLUDES = -I $(VDB_INC) -I $(OPENEXR_INC) -I $(MESHPOTATO_INC) -I $(OIIO_INC)
LIBS = -L$(VDB_LIB) -L$(MESHPOTATO_LIB) -L$(OIIO_LIB)

all: $(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) $(INCLUDES) $(LIBS) $(FLAGS) -o $(EXECUTABLE)

#g++ -g volumeTest.C -I ../../include -I /usr/include/OpenEXR -I /group/dpa/include/openvdb/include -L$HOME/projects/MeshPotato/lib -lmeshpotato -L/group/dpa/local/openvdb/lib -lImath -lboost_thread -lIex -lpthread -lz -lHalf -lopenvdb -lboost_system -lboost_filesystem -lboost_thread -ltbb -o volumeTest
