///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_SHAREDLIBRARY_H
#define MYENGINE_SHAREDLIBRARY_H

#include "Config.h"

#include <string>
#include <stdexcept>

#if defined(MYENGINE_WIN32)

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#elif defined(MYENGINE_LINUX)

#include <dlfcn.h>

#else

#error Please implement the shared library functions for your system

#endif

#include <stdio.h>
#include <stdlib.h>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <list>



namespace MyEngine {

  // ----------------------------------------------------------------------- //

#if defined(MYENGINE_WIN32)

  /// <summary>Shared library loading and access on windows</summary>
  class SharedLibrary {

    /// <summary>Handle by which DLLs are referenced</summary>
    public: typedef HMODULE HandleType;

    /// <summary>Loads the DLL from the specified path</summary>
    /// <param name="path">Path of the DLL that will be loaded</param>
    public: MYENGINE_API static HandleType Load(const std::string &path) {
      std::string pathWithExtension = path + ".dll";

      HMODULE moduleHandle = ::LoadLibraryA(pathWithExtension.c_str());
      if(moduleHandle == NULL) {
        throw std::runtime_error("Could not load DLL");
      }

      return moduleHandle;
    }

    /// <summary>Unloads the DLL with the specified handle</summary>
    /// <param name="sharedLibraryHandle">
    ///   Handle of the DLL that will be unloaded
    /// </param>
    public: MYENGINE_API static void Unload(HandleType sharedLibraryHandle) {
      BOOL result = ::FreeLibrary(sharedLibraryHandle);
      if(result == FALSE) {
        throw std::runtime_error("Could not unload DLL");
      }
    }

    /// <summary>Looks up a function exported by the DLL</summary>
    /// <param name="sharedLibraryHandle">
    ///   Handle of the DLL in which the function will be looked up
    /// </param>
    /// <param name="functionName">Name of the function to look up</param>
    /// <returns>A pointer to the specified function</returns>
    public: template<typename TSignature>
    static TSignature *GetFunctionPointer(
      HandleType sharedLibraryHandle, const std::string &functionName
    ) {
      FARPROC functionAddress = ::GetProcAddress(
        sharedLibraryHandle, functionName.c_str()
      );
      if(functionAddress == NULL) {
        throw std::runtime_error("Could not find exported function");
      }

      return reinterpret_cast<TSignature *>(functionAddress);
    }

  };

#endif

  // ----------------------------------------------------------------------- //

#if defined(MYENGINE_LINUX)

  /// <summary>Shared library loading and access on windows</summary>
  class SharedLibrary {

    /// <summary>Handle by which shared objects are referenced</summary>
    public: typedef void * HandleType;

public: MYENGINE_API static bool find_plugin(const boost::filesystem::path &dirpath,
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
                        if (find_plugin(*itr, filename, pluginfound)) return true;
                }


        }
        return false;
}

    /// <summary>Loads the shared object from the specified path</summary>
    /// <param name="path">
    ///   Path of the shared object that will be loaded
    /// </param>
    public: MYENGINE_API static HandleType Load(const std::string &path) {
      std::string pathWithExtension = std::string("lib") + path + ".so";

	std::list<std::string> paths;
	std::string libPath = getenv("MESHPOTATO_PLUGIN_PATH");
	boost::split(paths, libPath, boost::is_any_of(":"));
	boost::filesystem::path pluginPath;
	bool found = false;
	
      	void *sharedObject = NULL;
	for (std::list<std::string>::const_iterator iter = paths.begin(); iter != paths.end(); ++iter) {
		boost::filesystem::path mypath(*iter);
		found = find_plugin(mypath, pathWithExtension, pluginPath);
		if (found) {
			std::cout << "Found plugin " << pluginPath << std::endl;	
      			sharedObject = ::dlopen(pluginPath.string().c_str(), RTLD_NOW);
			if (sharedObject != NULL)
				break;
		}
		
	}



//      void *sharedObject = ::dlopen(pathWithExtension.c_str(), RTLD_NOW);
      if(sharedObject == NULL) {
        throw std::runtime_error(
          std::string("Could not load '") + pathWithExtension + "'"
        );
      }

      return sharedObject;
    }

    /// <summary>Unloads the shared object with the specified handle</summary>
    /// <param name="sharedLibraryHandle">
    ///   Handle of the shared object that will be unloaded
    /// </param>
    public: MYENGINE_API static void Unload(HandleType sharedLibraryHandle) {
      int result = ::dlclose(sharedLibraryHandle);
      if(result != 0) {
        throw std::runtime_error("Could not unload shared object");
      }
    }

    /// <summary>Looks up a function exported by the shared object</summary>
    /// <param name="sharedLibraryHandle">
    ///   Handle of the shared object in which the function will be looked up
    /// </param>
    /// <param name="functionName">Name of the function to look up</param>
    /// <returns>A pointer to the specified function</returns>
    public: template<typename TSignature>
    static TSignature *GetFunctionPointer(
      HandleType sharedLibraryHandle, const std::string &functionName
    ) {
      ::dlerror(); // clear error value

      void *functionAddress = ::dlsym(
        sharedLibraryHandle, functionName.c_str()
      );

      const char *error = ::dlerror(); // check for error
      if(error != NULL) {
        throw std::runtime_error("Could not find exported function");
      }

      return reinterpret_cast<TSignature *>(functionAddress);
    }

  };

#endif

  // ----------------------------------------------------------------------- //

} // namespace MyEngine

#endif // MYENGINE_SHAREDLIBRARY_H
