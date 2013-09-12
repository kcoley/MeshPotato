#ifndef MESHPOTATOPLUGIN_H
#define MESHPOTATOPLUGIN_H
#include <MeshPotato/meshpotato.h>
#include <dlfcn.h>
namespace MeshPotato
{
	namespace MPPlugins {
		typedef void *Handle;

		const char *plugin_extension(void);

		Handle open(const char *plugin_filename);

		bool close(Handle plugin_handle);

		void *getsym(Handle plugin_handle, const char *symbol_name);

		std::string geterror(void);


	}
}
#endif //MESHPOTATOPLUGIN_H
