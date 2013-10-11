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
	ABCOUTPUTPLUGIN_API virtual ~ABCOutputMeshDriver() {
		delete [] g_verts;
		delete [] g_indices;
		delete [] g_counts;
	}

			/// <summary>Gets the name of the graphics driver</summary>
	ABCOUTPUTPLUGIN_API virtual const std::string &getName() const {
				static std::string sName("ABC Output Mesh Driver");
				return sName;
	}

			/// <summary>Gets the name of the graphics driver</summary>
	ABCOUTPUTPLUGIN_API virtual void loadMesh(std::list<vertex> &vertices, std::list<vertex> &normals, std::list<vertex> &faces, MeshPotato::MeshSpec spec = MeshPotato::MeshSpec()) {

				int index = 0;
				this->vertices = vertices;
				this->normals = normals;
				this->faces = faces;
				
				// Convert to alembic friendly versions
				g_numVerts = vertices.size()*3;
				g_verts = new Alembic::Abc::float32_t[g_numVerts];
				//fill...
				
				for (std::list<vertex>::iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
					g_verts[index++] = itr->x;
					g_verts[index++] = itr->y;
					g_verts[index++] = itr->z;
				}
				g_numIndices = faces.size()*3; // change 3 to dynamically based on face size
				g_indices = new Alembic::Abc::int32_t[g_numIndices];
				//fill...
				index = 0;
				for (std::list<vertex>::iterator itr = faces.begin(); itr !=faces.end(); ++itr) {
					g_indices[index++] = itr->x;	
					g_indices[index++] = itr->y;	
					g_indices[index++] = itr->z;	
				}
				g_numCounts = faces.size();
				g_counts = new Alembic::Abc::int32_t[g_numCounts];	
				//fill...
				index = 0;
				for(unsigned int i = 0; i < g_numCounts; ++i) {
					g_counts[index++] = 3;
				}


	}
	ABCOUTPUTPLUGIN_API virtual bool writeMesh(const char *meshName) {
				// Open an Alembic file for writing
		Alembic::AbcGeom::OArchive archive(
			Alembic::AbcCoreHDF5::WriteArchive(),
			meshName);
//		Alembic::AbcGeom::OPolyMesh meshyObj(Alembic::AbcGeomOObject(archive, kTop), "mesh");
//		Alembic::AbcGeom::OPolyMeshSchema &mesh = meshyObj.getSchema();

//		OV2fGeomParam::Sample uvsamp(V2fArraySample((const V2f *)g_uvs, g_numUVs),kFacevaryingScope );
//		ON3fGeomParam::Sample nsamp(N3fArraySample((const N3f *)g_normals, g_numNormals),kFacevaryingScope);
/*
		OPolyMeshSchema::Sample mesh_samp(
			P3fArraySample((const V3f *)g_verts, g_numVerts),
			Int32ArraySample(g_indices, g_numIndices),
			Int32ArraySample(g_counts, g_numCounts));
//			,uvsamp, nsamp);

//		Box3d cbox;
//		cbox.extendBy(V3d(1.0, -1.0, 0.0));
//		cbox.extendBy(V3d(-1.0, 1.0, 3.0));

		mesh.set(mesh_samp);
		mesh.set(mesh_samp);

//		std::cout << "Writing: " << archive.getName() << std::endl;
*/
		return true;
			}

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
			std::auto_ptr<Mesh> createOutputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return std::auto_ptr<Mesh>(new Mesh());
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
