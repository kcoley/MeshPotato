/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for loading, unloading plugins
///
///   Copyright 2014 Kacey Coley
///   Licensed under the Apache License, Version 2.0 (the "License");
///   you may not use this file except in compliance with the License.
///   You may obtain a copy of the License at
///
///   http://www.apache.org/licenses/LICENSE-2.0
///
///   Unless required by applicable law or agreed to in writing, software
///   distributed under the License is distributed on an "AS IS" BASIS,
///   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
///   See the License for the specific language governing permissions and
///   limitations under the License.
///

#ifndef __PLUGIN_MANAGER_H__
#define __PLUGIN_MANAGER_H__

#include <string>
#include <vector>
#include "defines.h"

namespace MeshPotato {
namespace MPPlugins {
	/// Loads and unloads a plugin
class CORE_API PluginInstance {
public:
	explicit PluginInstance(const std::string &name);
	~PluginInstance();

	/// Load the plugin
	bool Load();
	/// Unload the plugin
	bool Unload();
	/// Return true if the plugin is loaded
	bool IsLoaded();

	/// Return the path to the plugin file on disk
	std::string GetFileName();
	/// Return the display name for the plugin
	std::string GetDisplayName();

private:
	// Make this object be noncopyable as it holds a pointer
	PluginInstance(const PluginInstance &);
	const PluginInstance &operator =(const PluginInstance &);

	class Impl;
	Impl *mImpl;
};

/// Singleton manager class for loading and unloading all plugins
class CORE_API PluginManager {
public:
	/// Return the single instance of the plugin manager
	static PluginManager &GetInstance();

	/// Load all plugins
	bool LoadAll();
	/// Load a single named plugin
	bool Load(const std::string &name);

	/// Unload all plugins
	bool UnloadAll();
	/// Unload a single named plugin
	bool Unload(const std::string &name);

	/// Return a list of all plugins
	std::vector<PluginInstance *> GetAllPlugins();

private:
	PluginManager();
	~PluginManager();
	PluginManager(const PluginManager &);
	const PluginManager &operator =(const PluginManager &);

	std::vector<PluginInstance *> mPlugins;


};
} // MPPlugins
} // MeshPotato
#endif
