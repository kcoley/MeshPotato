///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
//#define ABCOUTPUTPLUGIN_SOURCE 1

#include <MPPlugins/pluginapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <vector>
#include <MeshPotato/meshpotato.h>
using namespace MeshPotato::MPPlugins;
using namespace std;

// Alembic Includes
#include <Alembic/AbcGeom/All.h>
// This is required to tell Alembic which implementation to use.  In this case,
// the HDF5 implementation, currently the only one available.
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/Abc/ErrorHandler.h>
#include <assert.h>


//#include "../MyEngine/Utilities.h"
namespace MeshPotato {
	namespace MPMesh {


		class ABCOutputMesh : public OutputMesh {
		public:
			/// <summary>Gets the name of the mesh</summary>
			virtual const char* getName() const;
			virtual bool loadMesh(const std::list<std::vector<std::string> > &vertices,
			const std::list<std::vector<std::string> > &normals,
			const std::list<std::vector<std::string> > &faces,
			const MeshPotato::MPUtils::AttributeTable  &table);
			/// <summary>Destroys an OpenGL graphics driver</summary>
			~ABCOutputMesh();



			/// <summary>Gets the name of the graphics driver</summary>
			virtual bool loadMesh(std::list<std::vector<std::string> > &vertices,
			std::list<std::vector<std::string> > &normals,
			std::list<std::vector<std::string> > &faces,
			MeshPotato::MeshSpec spec);
			virtual bool writeMesh(const char *meshName);

			virtual const unsigned int getNumberVertices() const{
				return vertices.size();
			}
			virtual const unsigned int getNumberNormals() const{
				return normals.size();
			}
			virtual const unsigned int getNumberFaces() const{
				return faces.size();
			}


		private:
			std::list<std::vector<std::string> > vertices;
			std::list<std::vector<std::string> > normals;
			std::list<std::vector<std::string> > faces;
			size_t g_numVerts;
			Alembic::Abc::float32_t *g_verts;
			size_t g_numIndices;
			Alembic::Abc::int32_t *g_indices;
			size_t g_numCounts;
			Alembic::Abc::int32_t *g_counts;
			//Implement List here for dynamically growing amount of vertices
		};

		PLUGIN_FUNC OutputMesh *CreateOutputMesh() {
			return new ABCOutputMesh;
		}

		PLUGIN_FUNC void DestroyOutputMesh(OutputMesh * om) {
			delete om;
		}

		PLUGIN_DISPLAY_NAME("ABC OutputMesh");
		PLUGIN_INIT() {
			std::cout << "ABC_OUTPUT_PLUGIN_INIT" << std::endl;
			RegisterOutputMesh("abc", CreateOutputMesh, DestroyOutputMesh);
			return 0;
		}
	} // namespace MyPlugins
} // namespace MeshPotato
