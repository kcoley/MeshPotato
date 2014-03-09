#ifndef __PLUGINAPI_H__
#define __PLUGINAPI_H__
#include "defines.h"
#include "OutputMeshAPI.h"
#include "InputMeshAPI.h"
namespace MeshPotato {
namespace MPPlugins {
class OutputMeshAPI;
#define PLUGIN_API_VERSION 1

#define CORE_FUNC extern "C" CORE_API
#define PLUGIN_FUNC extern "C" PLUGIN_API

/// declaration of the initialization routine for a plugin
#define PLUGIN_INIT() \
	const int PluginVersion = PLUGIN_API_VERSION; \
	PLUGIN_FUNC int PluginInit()

/// declaration  of the cleanup routine for a plugin
#define PLUGIN_FREE() \
	PLUGIN_FUNC int PluginFree()

/// declaration of the display name of a plugin
#define PLUGIN_DISPLAY_NAME(name) \
	PLUGIN_API const char * PluginDisplayName = name

/// declaration of the minimum required Plugin API version for a plugin
#define PLUGIN_MIN_VERSION(version) \
	PLUGIN_API const char * PluginMinVersion = version

/// declaration of the maximum supported Plugin API version for a plugin
#define PLUGIN_MAX_VERSION(version) \
	PLUGIN_API const char * PluginMaxVersion = version

/// The function signature for a routine that writes out/reads in a mesh
typedef OutputMeshAPI *(*OutputMeshInitFunc)();
typedef InputMeshAPI *(*InputMeshInitFunc)();

/// The function signature for a routine that destroys an output mesh
typedef void (*OutputMeshFreeFunc)(OutputMeshAPI *);
typedef void (*InputMeshFreeFunc)(InputMeshAPI *);

/// A routine to let a plugin register a new output mesh type
CORE_FUNC void RegisterOutputMesh(const char *type, 
				  OutputMeshInitFunc init_cb,
				  OutputMeshFreeFunc free_cb);

CORE_FUNC void RegisterInputMesh(const char *type, 
				  InputMeshInitFunc init_cb,
				  InputMeshFreeFunc free_cb);

} // MPPlugins
} // MeshPotato
#endif // __PLUGINAPI_H__
