#CC=g++
CC=g++
CFLAGS=-c -Wall -fPIC

INCLUDES= -I ../../include -I /home/kcoley/Downloads/Alembic_150/Alembic_1.5.0_2013072300/lib -I /home/kcoley/projects/usr/local/include/OpenEXR
LINKS= -L/home/kcoley/projects/alembic/lib -L/home/kcoley/projects/alembic/lib/Alembic/Abc/ -L/home/kcoley/projects/alembic/lib/Alembic/AbcGeom -L/home/kcoley/projects/alembic/lib/Alembic/AbcCoreHDF5 -L/home/kcoley/projects/alembic/lib/Alembic/AbcCollection -L/home/kcoley/projects/alembic/lib/Alembic/AbcMaterial -L/home/kcoley/projects/alembic/lib/AbcOpenGL -L/home/kcoley/projects/alembic/lib/Alembic/AbcCoreAbstract -L/home/kcoley/projects/alembic/lib/Alembic/Util -lAlembicAbc -lAlembicAbcGeom -lAlembicAbcCoreHDF5 -lAlembicAbcCollection -lAlembicAbcMaterial -lAlembicAbcOpenGL -lImath -lIex -lboost_thread -lhdf5_hl -lhdf5 -lpthread -lz -lAlembicAbcCoreAbstract -lAlembicUtil -lHalf


PLUGINFILES=$(*.C)
PLUGINNAME=libABCOutputPlugin.so


all: $(PLUGINFILES)
	$(CC) -fPIC -c $(CFLAGS) $(INCLUDES) ABCOutputPlugin.C -o mpabcoutputplugin.o
	$(CC) -shared -Wl,-soname,$(PLUGINNAME) -o $(PLUGINNAME) mpabcoutputplugin.o

clean:
	rm *.o
	rm *.so
