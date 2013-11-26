///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_KERNEL_H
#define MYENGINE_KERNEL_H

#include "Config.h"
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


    /// <summary>Accesses the graphics server</summary>

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
	Plugin plugin(filename);
//	std::cout << "Extension = " << std::endl;
	try {
	std::cout << "Extension = " << plugin.getExtension() << std::endl;
	} catch(int e) {
	std::cout << "could not get extension" << std::endl;
	}
      if(this->loadedPlugins.find(plugin.getExtension()) == this->loadedPlugins.end()) {
        this->loadedPlugins.insert(PluginMap::value_type(plugin.getExtension(), plugin)).first->second.registerPlugin(*this);
	std::cout << "Added plugin " << plugin.getExtension() << " to pluginmap" << std::endl;
      }
    }

    /// <summary>All plugins currently loaded</summary>
    private:
    PluginMap loadedPlugins;
    /// <summary>Manages storage-related tasks for the engine</summary>
    InputMesh inputMesh;
    OutputMesh outputMesh;

  };

} // namespace MyEngine

#endif // MYENGINE_KERNEL_H
