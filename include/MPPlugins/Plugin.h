///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_PLUGIN_H
#define MYENGINE_PLUGIN_H

#include "Config.h"
#include "SharedLibrary.h"

#include <string>

namespace MyEngine {

  // ----------------------------------------------------------------------- //

  class Kernel;

  // ----------------------------------------------------------------------- //

  /// Representation of a plugin
  class Plugin {

    /// <summary>Signature for the version query function</summary>
    private: typedef int GetEngineVersionFunction();
    /// <summary>Signature for the plugin's registration function</summary>
    private: typedef void RegisterPluginFunction(Kernel &);

    /// <summary>Initializes and loads a plugin</summary>
    /// <param name="filename">Filename of the plugin to load</summary>
    public: MYENGINE_API Plugin(const std::string &filename);
    /// <summary>Copies an existing plugin instance</summary>
    public: MYENGINE_API Plugin(const Plugin &other);
    /// <summary>Unloads the plugin</summary>
    public: MYENGINE_API ~Plugin();

    /// <summary>Queries the plugin for its expected engine version</summary>
    public: MYENGINE_API int getEngineVersion() const {
      return this->getEngineVersionAddress();
    }

    /// <summary>Register the plugin to a kernel</summary>
    /// <param name="kernel">Kernel the plugin should register to</param>
    public: MYENGINE_API void registerPlugin(Kernel &kernel) {
      this->registerPluginAddress(kernel);
    }

    /// <summary>Creates a copy of the plugin instance</summary>
    public: Plugin &operator =(const Plugin &other);

    /// <summary>Handle of the loaded shared library</summary>
    private: SharedLibrary::HandleType sharedLibraryHandle;
    /// <summary>Number of references that exist to the shared library</summary>
    private: size_t *referenceCount;
    /// <summary>Function to query for the expected engine version</summary>
    private: GetEngineVersionFunction *getEngineVersionAddress;
    /// <summary>Registers the plugin with the kernel</summary>
    private: RegisterPluginFunction *registerPluginAddress;

  };

  // ----------------------------------------------------------------------- //

} // namespace MyEngine

#endif // MYENGINE_PLUGIN_H
