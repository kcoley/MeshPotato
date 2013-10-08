///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used                                        //
///////////////////////////////////////////////////////////////////////////////
#ifndef MYENGINE_OUTPUTMESH_H
#define MYENGINE_OUTPUTMESH_H


#include "Config.h"
#include "Utilities.h"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "MeshPotato/meshpotato.h"
namespace MyEngine {

  // ----------------------------------------------------------------------- //

//  class Renderer {}; // Dummy
//  class Mesh {}; // Dummy

  // ----------------------------------------------------------------------- //

  /// <summary>Manages graphics related stuff for the engine</summary>
//  class GraphicsServer {
  class OutputMesh {

    /// <summary>Graphics driver interface for the engine</summary>
//    public: class GraphicsDriver {
    public: class OutputMeshDriver {
      /// <summary>Releases resources of a driver after use</summary>
//      public: virtual ~GraphicsDriver() {}
      public: virtual ~OutputMeshDriver() {}
      /// <summary>Gets the name of the graphics driver</summary>
      public: virtual const std::string &getName() const = 0;
      /// <summary>Gets the name of the graphics driver</summary>
      public: virtual void loadMesh(std::list<vertex> &, std::list<vertex> &, std::list<vertex> &, MeshPotato::MeshSpec spec = MeshPotato::MeshSpec())  = 0;
      public: virtual bool writeMesh(const char *)  = 0;
      public: virtual const size_t getNumberVertices() const = 0;
      public: virtual const size_t getNumberNormals() const = 0;
      public: virtual const size_t getNumberFaces() const = 0;
//      public: virtual size_t getNumVertices()  = 0;
      /// <summary>Creates a renderer using the driver's rendering API</summary>
      /// <summary>Creates a renderer using the driver's rendering API</summary>
//      public: virtual std::auto_ptr<Renderer> createRenderer() = 0;
      public: virtual std::auto_ptr<Mesh> createOutputMesh() = 0;
    };

    /// <summary>A vector of graphics drivers</summary>
//    private: typedef std::vector<GraphicsDriver *> GraphicsDriverVector;
//    private: typedef std::vector<OutputMeshDriver *> OutputMeshVector;
    private: typedef std::map<std::string, OutputMeshDriver *> OutputMeshMap;

    /// <summary>Releases the resources of the graphics server</summary>
//    public: MYENGINE_API ~GraphicsServer() {
    public: MYENGINE_API ~OutputMesh() {
      for(
//        GraphicsDriverVector::const_iterator it = this->graphicsDrivers.begin();
        OutputMeshMap::const_iterator it = this->outputMeshDrivers.begin();
//        it != this->graphicsDrivers.end();
        it != this->outputMeshDrivers.end();
        ++it
      ) {
        delete it->second;
      }
    }

    /// <summary>Allows plugins to add new graphics drivers</summary>
    /// <param name="graphicsDriver">Graphics driver that will be added</param>
//    public: MYENGINE_API void addGraphicsDriver(
    public: MYENGINE_API void addOutputMeshDriver(
	std::string extension,
//      std::auto_ptr<GraphicsDriver> graphicsDriver
      std::auto_ptr<OutputMeshDriver> outputMeshDriver
    ) {
//      this->graphicsDrivers.push_back(graphicsDriver.release());
      this->outputMeshDrivers[extension] = outputMeshDriver.release();
    }

    /// <summary>Gets the total number of registered graphics drivers</summary>
    public: MYENGINE_API size_t getDriverCount() const {
//      return this->graphicsDrivers.size();
      return this->outputMeshDrivers.size();
    }

    /// <summary>Accesses a driver by its index</summary>
//    public: MYENGINE_API GraphicsDriver &getDriver(size_t Index) {
    public: MYENGINE_API OutputMeshDriver &getDriver(std::string ext) {
//      return *this->graphicsDrivers.at(Index);
      return *this->outputMeshDrivers.find(ext)->second;
    }

    /// <summary>All available graphics drivers</summary>
//    private: GraphicsDriverVector graphicsDrivers;
//    private: OutputMeshVector outputMeshDrivers;
    private: OutputMeshMap outputMeshDrivers;

  };

  // ----------------------------------------------------------------------- //

} // namespace MyEngine

//#endif // MYENGINE_GRAPHICSSERVER_H
#endif // MYENGINE_OUTPUTMESH_H
