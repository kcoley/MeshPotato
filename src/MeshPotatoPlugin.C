#include <MPPlugins/meshpotatoplugin.h>

namespace MeshPotato
{
	namespace MPPlugins
	{
		typedef void *Handle;
		//! Only supports linux plugins for now...
		const char* plugin_extension(void)
		{
				return "so";
		}


		Handle dlopen (const char *plugin_filename, int)
		{
			return dlopen(plugin_filename, RTLD_LAZY);
		}

		bool dlclose (MeshPotato::MPPlugins::Handle plugin_handle)
		{
			return dlclose(plugin_handle) !=0;
		}

		void *
			dlsym (Handle plugin_handle, const char *symbol_name)
			{
				return NULL;
			}

/*
		Handle
			MeshPotato::MPPlugin::open (const char *plugin_filename, bool global)
			{
				int mode = RTLD_LAZY;
				if (global)
					mode |= RTLD_GLOBAL;
				Handle h = dlopen (plugin_filename, mode);
				return h;
			}


		bool
			MeshPotato::MPPlugin::close (Handle plugin_handle)
			{
				if (dlclose (plugin_handle)) {
					return false;
				}
				return true;
			}



		void *
			MeshPotato::MPPlugin::getsym (Handle plugin_handle, const char *symbol_name)
			{
				void *s = dlsym (plugin_handle, symbol_name);
				return s;
			}


		std::string
			MeshPotato::MPPlugin::geterror (void)
			{
				std::string e = last_error;
				return e;
			}


*/
	}
}
