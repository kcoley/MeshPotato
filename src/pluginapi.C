#include "MeshPotato/MPPlugins/coreapi.h"
#include "MeshPotato/MPPlugins/pluginapi.h"

namespace MeshPotato {
	namespace MPPlugins {

		void RegisterOutputMesh(const char *type,
				OutputMeshInitFunc init_cb,
				OutputMeshFreeFunc free_cb) {
			OutputMeshFactory::RegisterOutputMesh(type, init_cb);
			(void) free_cb; // objects never get freed
		}
		void RegisterInputMesh(const char *type,
				InputMeshInitFunc init_cb,
				InputMeshFreeFunc free_cb) {
			InputMeshFactory::RegisterInputMesh(type, init_cb);
			(void) free_cb; // objects never get freed
		}
		void RegisterVolumeFloat(const char *type,
				VolumeFloatInitFunc init_cb,
				VolumeFloatFreeFunc free_cb) {
			VolumeFloatFactory::RegisterVolumeFloat(type, init_cb);
			(void) free_cb; // objects never get freed
		}
	}
}
