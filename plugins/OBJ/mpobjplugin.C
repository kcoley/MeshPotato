#include "mpobjplugin.h"

extern "C" MeshPotato::MPPlugins::OBJInput* create() {
	return new MeshPotato::MPPlugins::OBJInput;
}

extern "C" void destroy(MeshPotato::MPPlugins::OBJInput* obj) {
	delete obj;
}
