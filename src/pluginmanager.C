#include "MPPlugins/pluginmanager.h"
#include <string>
#include <map>
#include <set>
#include <dlfcn.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
namespace MeshPotato {
namespace MPPlugins {
class PluginInstance::Impl {
	public:
		std::string mFileName;
		std::string mDisplayName;

		// Plugin loading routine for Mac & Linux
		void *handle;

		typedef void (*PluginFunc)();

		bool Load() {
			std::list <std::string> paths;
			std::string libPath = getenv("MESHPOTATO_PLUGIN_PATH");
			boost::split(paths, libPath, boost::is_any_of(":"));
			boost::filesystem::path pluginPath;

			for (std::list<std::string>::const_iterator iter = paths.begin();
				iter != paths.end(); ++iter) {
				boost::filesystem::path mypath(*iter);
				if (Find(mypath, mFileName, pluginPath)) {
					handle = dlopen(pluginPath.c_str(), RTLD_LOCAL | RTLD_LAZY);
					break;
				}
			}
			return (handle != NULL);
		}
		bool Unload() {
			if (!handle)
				return true;
			return dlclose(handle) == 0;
		}
		PluginFunc GetFunction(const std::string &name) {
			dlerror();
			PluginFunc fptr = (PluginFunc) dlsym(handle, name.c_str());
			if (dlerror())
				return NULL;
			return fptr;
		}

		bool Find(const boost::filesystem::path &dirpath,
				const std::string &filename,
				boost::filesystem::path &pluginfound) {
				if (!boost::filesystem::exists(dirpath)) return false;
 				       boost::filesystem::directory_iterator end_itr;
				        boost::filesystem::path mypath;
			        for (boost::filesystem::directory_iterator itr( dirpath); itr != end_itr; ++itr)
			        {
     			           mypath = *itr;
  			              if (mypath.filename() == filename) {
         		               pluginfound = mypath;
                 		       return true;
            			    }
            			    if (boost::filesystem::is_directory(*itr)) {
                  		      if (Find(*itr, filename, pluginfound)) return true;
               			 }


      				  }
       		 return false;

	}


};

PluginInstance::PluginInstance(const std::string &name) {
	mImpl = new Impl;
	mImpl->mDisplayName = name;
#if __APPLE__
	mImpl->mFileName = "lib" + name + ".dylib";
#else
	// Linux
	mImpl->mFileName = "lib" + name + ".so";
#endif
}

PluginInstance::~PluginInstance() {
	delete mImpl;
}

bool PluginInstance::Load()
{
	if (! mImpl->Load()) {
		return false;
	}

	Impl::PluginFunc init_func = mImpl->GetFunction("PluginInit");
	if (!init_func) {
		return false;
	}

	(*init_func)();

	Impl::PluginFunc name_string = mImpl->GetFunction("PluginDisplayName");
	if (name_string)
	{
		const char **ptr = (const char **) name_string;
		mImpl->mDisplayName = *ptr;
	}

	return true;
}
/*
bool PluginInstance::Find(const boost::filesystem::path &dirpath, 
		const std::string &filename, 
		boost::filesystem::path &pluginfound) {
	if (!boost::filesystem::exists(dirpath)) return false;
	boost::filesystem::directory_iterator end_itr;
	boost::filesystem::path mypath;
	for (boost::filesystem::directory_iterator itr( dirpath); itr != end_itr; ++itr)
	{
		mypath = *itr;
		if (mypath.filename() == filename) {
			pluginfound = mypath;
			return true;
		}
		if (boost::filesystem::is_directory(*itr)) {
			if (Find(*itr, filename, pluginfound)) return true;
		}


	}
	return false;
}
*/



bool PluginInstance::Unload()
{
	return mImpl->Unload();
}

bool PluginInstance::IsLoaded()
{
	return (mImpl->handle != NULL);
}

std::string PluginInstance::GetFileName()
{
	return mImpl->mFileName;
}

std::string PluginInstance::GetDisplayName()
{
	return mImpl->mDisplayName;
}

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
}

PluginManager &PluginManager::GetInstance()
{
	static PluginManager mgr;
	return mgr;
}
bool PluginManager::LoadAll()
{

	// hardcode loading our 2 example plugins
	// in reality, this would search for all plugins to load
	Load("OBJOutputPlugin");
	Load("OBJInputPlugin");
//	Load("VDBOutputPlugin");
	return true;
}

bool PluginManager::Load(const std::string &name)
{
	// have we already loaded this plugin?
	static std::set<std::string> loaded;
	if (loaded.find(name) != loaded.end())
		return true;

	// try to load this plugin
	PluginInstance *pi = new PluginInstance(name);
	if (! pi->Load())
	{
		delete pi;
		return false;
	}

	// success! add the plugin to our manager
	mPlugins.push_back(pi);

	// and don't try to load it again
	loaded.insert(name);
	return true;
}
bool PluginManager::UnloadAll()
{
	// not implemented yet
	return false;
}

bool PluginManager::Unload(const std::string &name)
{
	// not implemented yet
	return false;
}

std::vector<PluginInstance *> PluginManager::GetAllPlugins()
{
	// If we had an external metadata file that described all of the
	// plugins, this would be the point where we load that file -
	// without loading the actual plugins.  Without that external
	// metadata, we need to load all plugins.
	LoadAll();
	return mPlugins;
}
} // MPPlugins
} // MeshPotato
