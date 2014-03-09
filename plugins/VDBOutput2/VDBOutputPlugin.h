#include <MPPlugins/pluginapi.h>
#include <MPUtils/Vector.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
using namespace std;
namespace MeshPotato {
	namespace MPPlugins {
	class VDBOutputMesh : public OutputMeshAPI {
		public:
			virtual ~VDBOutputMesh();
			virtual const char *getName() const;
			virtual bool loadMesh(const list<std::vector<std::string> > &vertices, const list<std::vector<std::string> >&normals, const list<std::vector<std::string> > &faces, const MeshPotato::MeshSpec &spec);
			virtual bool writeMesh(const char *meshName);
			virtual const unsigned int getNumberVertices() const;
			virtual const unsigned int getNumberNormals() const;
			virtual const unsigned int getNumberFaces() const;
			virtual void setVoxelSize(float );

		private:
			float voxelSize;
			float exBandWidth;
			float inBandWidth;
			list<std::vector<std::string> > vertices;
			list<std::vector<std::string> > normals;
			list<std::vector<std::string> > faces;
			openvdb::FloatGrid::Ptr grid;


	};
	PLUGIN_FUNC OutputMeshAPI *CreateOutputMesh () {
		return new VDBOutputMesh;
	}
	PLUGIN_FUNC void DestroyOutputMesh(OutputMeshAPI *om) {
		delete om;
	}
	PLUGIN_DISPLAY_NAME("VDB OutputMesh");
	PLUGIN_INIT() {
		std::cout << "PLUGIN_INIT_VDB_OUT" << std::endl;
		RegisterOutputMesh("vdb", CreateOutputMesh, DestroyOutputMesh);
		return 0;
	}

	}

}
