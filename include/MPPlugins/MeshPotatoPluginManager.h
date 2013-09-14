#include <MPPlugins/meshpotatoplugin.h>
#include <memory>
namespace MeshPotato {
	namespace MPPlugins {
		class PluginManager {
			void registerMesh(std::auto_ptr<InputMeshFactory> IMF);
		};
	}
}
