#CC=g++
CC=clang++
CFLAGS=-c -Wall -fPIC
LDFLAGS=
SOURCES=src/MPMesh.C \
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

#plugins: $(PLUGINFILES)
#	$(CC) -fPIC -c -I ./include -I ./plugins -L./lib $(PLUGINFILES) -o plugins/OBJInput/mpobjinputplugin.o
#	$(CC) -shared -Wl,-soname,mpobjinputplugin.so -o plugins/OBJInput/mpobjinputplugin.so plugins/OBJInput/mpobjinputplugin.o
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
