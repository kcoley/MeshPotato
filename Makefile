CC=g++
CFLAGS=-c -Wall -fPIC
LDFLAGS=
SOURCES=src/MPMesh.C \
src/Vector.C \
src/MeshPotato.C 
OBJECTS=$(SOURCES:.C=.o)
EXECUTABLE=transformer
PLUGINS=plugins
#Paths for OpenVDB
VDBINCLUDE=${HOME}/projects/OpenVDB/include
VDBLIB=${HOME}/projects/OpenVDB/lib

INCLUDES= -I ./include -I $(VDBINCLUDE)
LINKS= -L$(VDBLIB) -lopenvdb
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

plugins: $(PLUGINFILES)
	$(CC) -fPIC -c -I ./include ./plugins -L./lib $(PLUGINFILES) -o plugins/OBJ/mpobjplugin.o
	$(CC) -shared -Wl,-soname,mpobj.so -o plugins/OBJ/mpobj.so plugins/OBJ/mpobjplugin.o
clean:
	rm src/*.o
	rm lib/*.so

cleanplugins:
	rm plugins/*/*.o
	rm plugins/*/*.so
