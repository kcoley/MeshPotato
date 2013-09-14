#ifndef MESHPOTATOPLUGIN_H
#define MESHPOTATOPLUGIN_H
#include <MeshPotato/meshpotato.h>
#include <dlfcn.h>
namespace MeshPotato
{
	namespace MPPlugins {
		template<typename Interface>
		class Factory {
			virtual Interface *create() = 0;
		};
		class InputMesh {
			virtual void open() = 0;
			virtual void close() = 0;

		};
		typedef Factory<InputMesh> InputMeshFactory;
	}
}
#endif //MESHPOTATOPLUGIN_H
