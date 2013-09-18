///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#define MYENGINE_SOURCE 1

#include <MPPlugins/Plugin.h>

#include <stdexcept>

using namespace std;

namespace MyEngine {

  // ----------------------------------------------------------------------- //

  Plugin::Plugin(const std::string &filename) :
    sharedLibraryHandle(0),
    referenceCount(0),
    getEngineVersionAddress(0),
    registerPluginAddress(0) {

    // Try to load the plugin as a shared library
    this->sharedLibraryHandle = SharedLibrary::Load(filename);

    // Locate the plugin's exported functions
    try {
      this->getEngineVersionAddress = SharedLibrary::GetFunctionPointer<
        GetEngineVersionFunction
      >(this->sharedLibraryHandle, "getEngineVersion");
      this->registerPluginAddress = SharedLibrary::GetFunctionPointer<
        RegisterPluginFunction
      >(this->sharedLibraryHandle, "registerPlugin");

      // Initialize a new shared library reference counter
      this->referenceCount = new size_t(1);
    }
    catch(std::exception &) {
      SharedLibrary::Unload(this->sharedLibraryHandle);
      throw;
    }
  }

  /// <summary>
  ///   Creates a copy of a plugin that has already been loaded.
  ///   Required to provide correct semantics for storing plugins in
  ///   an STL map container.
  /// </summary>
  /// <param name="other">Other plugin instance to copy</param>
  Plugin::Plugin(const Plugin &other) :
    sharedLibraryHandle(other.sharedLibraryHandle),
    referenceCount(other.referenceCount),
    getEngineVersionAddress(other.getEngineVersionAddress),
    registerPluginAddress(other.registerPluginAddress) {

    // Increase DLL reference counter
    if(this->referenceCount) {
      ++(*this->referenceCount);
    }
  }

  /// <summary>
  ///   Destroys the plugin, unloading its library when no more references
  ///   to it exist.
  /// </summary>
  Plugin::~Plugin() {
    int remainingReferences = --*(this->referenceCount);
    if(remainingReferences == 0) {
      delete this->referenceCount;
      SharedLibrary::Unload(this->sharedLibraryHandle);
    }
  }

} // namespace MyEngine
