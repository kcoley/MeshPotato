#include "defines.h"
#include "OutputMesh.h"
#include "InputMesh.h"

#define CORE_FUNC extern "C" CORE_API
#define PLUGIN_FUNC extern "C" PLUGIN_API

#define PLUGIN_INIT() PLUGIN_FUNC int PluginInit()
#define PLUGIN_FREE() PLUGIN_FUNC int PluginFree()

typedef OutputMesh *(*OutputMeshInitFunc)();
typedef void (*OutputMeshFreeFunc)(OutputMesh *);

CORE_FUNC void RegisterOutputMesh(const char *type, OutputMeshInitFunc int cb,
				  OutputMeshFreeFunc free cb);

CORE_FUNC void RegisterInputMesh(const char *type, InputMeshInitFunc int cb,
				  InputMeshFreeFunc free cb);
