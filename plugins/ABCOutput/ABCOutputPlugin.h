///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
//#define ABCOUTPUTPLUGIN_SOURCE 1

#include "Config.h"
#include <MPPlugins/Kernel.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>

// Alembic Includes
#include <Alembic/AbcGeom/All.h>
// This is required to tell Alembic which implementation to use.  In this case,
// the HDF5 implementation, currently the only one available.
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/Abc/ErrorHandler.h>
#include <assert.h>


//#include "../MyEngine/Utilities.h"
namespace MyEngine {

	/// OpenGL graphics drver
	//  class OpenGLGraphicsDriver : public GraphicsServer::GraphicsDriver {
	class ABCOutputMeshDriver : public OutputMesh::OutputMeshDriver {
	public:
		/// <summary>Destroys an OpenGL graphics driver</summary>
	ABCOUTPUTPLUGIN_API virtual ~ABCOutputMeshDriver(); 

			/// <summary>Gets the name of the graphics driver</summary>
	ABCOUTPUTPLUGIN_API virtual const std::string &getName() const;

			/// <summary>Gets the name of the graphics driver</summary>
	ABCOUTPUTPLUGIN_API virtual void loadMesh(std::list<vertex> &vertices, std::list<vertex> &normals, std::list<vertex> &faces, MeshPotato::MeshSpec spec);
	ABCOUTPUTPLUGIN_API virtual bool writeMesh(const char *meshName);

	ABCOUTPUTPLUGIN_API virtual const size_t getNumberVertices() const{
				return vertices.size();
			}
	ABCOUTPUTPLUGIN_API virtual const size_t getNumberNormals() const{
				return normals.size();
			}
	ABCOUTPUTPLUGIN_API virtual const size_t getNumberFaces() const{
				return faces.size();
			}


			/// <summary>Creates a renderer</summary>
			//    auto_ptr<Mesh> createRenderer() {
			std::auto_ptr<MPMesh> createOutputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return std::auto_ptr<MPMesh>(new MPMesh());
			}
	private:
			std::list<vertex> vertices;
			std::list<vertex> normals;
			std::list<vertex> faces;
			size_t g_numVerts;
			Alembic::Abc::float32_t *g_verts;
			size_t g_numIndices;
			Alembic::Abc::int32_t *g_indices;
			size_t g_numCounts;
			Alembic::Abc::int32_t *g_counts;
			//Implement List here for dynamically growing amount of vertices
	};

	/// <summary>Retrieve the file extension we're going to expect</summary>
	extern "C" ABCOUTPUTPLUGIN_API const std::string getExtension() {
                return "abc_output";
        }

	/// <summary>Retrieve the engine version we're going to expect</summary>
	extern "C" ABCOUTPUTPLUGIN_API int getEngineVersion() {
		return 1;
	}

	/// <summary>Register the plugin to an engine kernel</summary>
	/// <param name="kernel">Kernel the plugin will register to</summary>
	extern "C" ABCOUTPUTPLUGIN_API void registerPlugin(Kernel &kernel) {
		kernel.getOutputMesh().addOutputMeshDriver("abc",
				std::auto_ptr<OutputMesh::OutputMeshDriver>(new ABCOutputMeshDriver())
				);
	}

} // namespace MyEngine
