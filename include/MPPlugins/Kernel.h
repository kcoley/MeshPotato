///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_KERNEL_H
#define MYENGINE_KERNEL_H

#include "Config.h"
#include "StorageServer.h"
#include "GraphicsServer.h"
#include "InputMesh.h"
#include "OutputMesh.h"
#include "Plugin.h"
#include "Utilities.h"
#include <string>
#include <map>

namespace MyEngine {

  /// <summary>The engine's core</summary>
  class Kernel {

    /// <summary>Map of plugins by their associated file names</summary>
    typedef std::map<std::string, Plugin> PluginMap;

    /// <summary>Accesses the storage server</summary>
    public: MYENGINE_API StorageServer &getStorageServer() {
      return this->storageServer;
    }

    /// <summary>Accesses the graphics server</summary>
    public: MYENGINE_API GraphicsServer &getGraphicsServer() {
      return this->graphicsServer;
    }

    /// <summary>Accesses the graphics server</summary>
    public: MYENGINE_API InputMesh &getInputMesh() {
      return this->inputMesh;
    }
    /// <summary>Accesses the graphics server</summary>
    public: MYENGINE_API OutputMesh &getOutputMesh() {
      return this->outputMesh;
    }

    /// <summary>Loads a plugin</summary>
    /// <param name="filename">File the plugin will be loaded from</param>
    public: MYENGINE_API void loadPlugin(const std::string &filename) {
      if(this->loadedPlugins.find(filename) == this->loadedPlugins.end()) {
        this->loadedPlugins.insert(
          PluginMap::value_type(filename, Plugin(filename))
        ).first->second.registerPlugin(*this);
      }
    }

    /// <summary>All plugins currently loaded</summary>
    private: PluginMap loadedPlugins;
    /// <summary>Manages storage-related tasks for the engine</summary>
    private: StorageServer storageServer;
    /// <summary>Manages storage-related tasks for the engine</summary>
    private: InputMesh inputMesh;
    private: OutputMesh outputMesh;
    /// <summary>Manages graphics-related tasks for the engine</summary>
    /// <summary>Manages graphics-related tasks for the engine</summary>
    private: GraphicsServer graphicsServer; 

  };

} // namespace MyEngine

#endif // MYENGINE_KERNEL_H
