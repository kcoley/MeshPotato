///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_INPUTMESH_H
#define MYENGINE_INPUTMESH_H


#include "Config.h"

#include <vector>
#include <string>
#include "Utilities.h"
#include <map>
#include <list>
#include <memory>
namespace MyEngine {

  // ----------------------------------------------------------------------- //

//  class Renderer {}; // Dummy
  class MPMesh {}; // Dummy

  // ----------------------------------------------------------------------- //

  /// <summary>Manages graphics related stuff for the engine</summary>
//  class GraphicsServer {
  class InputMesh {

    /// <summary>Graphics driver interface for the engine</summary>
//    public: class GraphicsDriver {
    public: class InputMeshDriver {
      /// <summary>Releases resources of a driver after use</summary>
//      public: virtual ~GraphicsDriver() {}
      public: virtual ~InputMeshDriver() {}
      /// <summary>Gets the name of the graphics driver</summary>
      public: virtual const std::string &getName() const = 0;
      /// <summary>Gets the name of the graphics driver</summary>
      public: virtual void loadMesh(const char *)  = 0;
      public: virtual const size_t getNumberVertices() const = 0;
      public: virtual const std::list<std::vector<std::string> > getVertices() const = 0;
      public: virtual const size_t getNumberNormals() const = 0;
      public: virtual const std::list<std::vector<std::string> > getNormals() const = 0;
      public: virtual const size_t getNumberFaces() const = 0;
      public: virtual const std::list<std::vector<std::string> > getFaces() const = 0;
//      public: virtual size_t getNumVertices()  = 0;
      /// <summary>Creates a renderer using the driver's rendering API</summary>
      /// <summary>Creates a renderer using the driver's rendering API</summary>
//      public: virtual std::auto_ptr<Renderer> createRenderer() = 0;
      public: virtual std::auto_ptr<MPMesh> createInputMesh() = 0;
    };

    /// <summary>A vector of graphics drivers</summary>
//    private: typedef std::vector<GraphicsDriver *> GraphicsDriverVector;
//    private: typedef std::vector<InputMeshDriver *> InputMeshVector;
    private: typedef std::map<std::string, InputMeshDriver *> InputMeshMap;

    /// <summary>Releases the resources of the graphics server</summary>
//    public: MYENGINE_API ~GraphicsServer() {
    public: MYENGINE_API ~InputMesh() {
      for(
//        GraphicsDriverVector::const_iterator it = this->graphicsDrivers.begin();
//        InputMeshVector::const_iterator it = this->inputMeshDrivers.begin();
        InputMeshMap::const_iterator it = this->inputMeshDrivers.begin();
//        it != this->graphicsDrivers.end();
        it != this->inputMeshDrivers.end();
        ++it
      ) {
        delete it->second;
      }
    }

    /// <summary>Allows plugins to add new graphics drivers</summary>
    /// <param name="graphicsDriver">Graphics driver that will be added</param>
//    public: MYENGINE_API void addGraphicsDriver(
    public: MYENGINE_API void addInputMeshDriver(
	std::string extension,
//      std::auto_ptr<GraphicsDriver> graphicsDriver
      std::auto_ptr<InputMeshDriver> inputMeshDriver
    ) {
//      this->graphicsDrivers.push_back(graphicsDriver.release());
//      this->inputMeshDrivers.push_back(inputMeshDriver.release());
      this->inputMeshDrivers[extension] = inputMeshDriver.release();
//      this->inputMeshDrivers.insert(extension, inputMeshDriver.release());
    }

    /// <summary>Gets the total number of registered graphics drivers</summary>
    public: MYENGINE_API size_t getDriverCount() const {
//      return this->graphicsDrivers.size();
      return this->inputMeshDrivers.size();
    }

    /// <summary>Accesses a driver by its index</summary>
//    public: MYENGINE_API GraphicsDriver &getDriver(size_t Index) {
    public: MYENGINE_API InputMeshDriver &getDriver(std::string ext) {
//      return *this->graphicsDrivers.at(Index);
      return *this->inputMeshDrivers.find(ext)->second;
    }

    /// <summary>All available graphics drivers</summary>
//    private: GraphicsDriverVector graphicsDrivers;
//    private: InputMeshVector inputMeshDrivers;
    private: InputMeshMap inputMeshDrivers;

  };

  // ----------------------------------------------------------------------- //

} // namespace MyEngine

//#endif // MYENGINE_GRAPHICSSERVER_H
#endif // MYENGINE_INPUTMESH_H
