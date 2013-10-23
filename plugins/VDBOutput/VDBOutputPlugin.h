#include "Config.h"
#include <MPPlugins/Kernel.h>
#include <MPUtils/Vector.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
using namespace std;
namespace MyEngine {
	class VDBOutputMeshDriver : public OutputMesh::OutputMeshDriver {
		public:
			VDBOUTPUTPLUGIN_API virtual ~VDBOutputMeshDriver();
			VDBOUTPUTPLUGIN_API virtual const std::string &getName() const;
			VDBOUTPUTPLUGIN_API virtual void loadMesh(list<vertex> &vertices, list<vertex> &normals, list<vertex> &faces, MeshPotato::MeshSpec spec = MeshPotato::MeshSpec());
			VDBOUTPUTPLUGIN_API virtual bool writeMesh(const char *meshName);
			VDBOUTPUTPLUGIN_API virtual const size_t getNumberVertices() const;
			VDBOUTPUTPLUGIN_API virtual const size_t getNumberNormals() const;
			VDBOUTPUTPLUGIN_API virtual const size_t getNumberFaces() const;
			VDBOUTPUTPLUGIN_API virtual void setVoxelSize(float );

			auto_ptr<MPMesh> createOutputMesh();
		private:
			float voxelSize;
			float exBandWidth;
			float inBandWidth;
			list<vertex> vertices;
			list<vertex> normals;
			list<vertex> faces;
			openvdb::FloatGrid::Ptr grid;


	};
	/// <summary>Retrieve the file extension we're going to expect</summary>
	extern "C" VDBOUTPUTPLUGIN_API const std::string getExtension() {
		return "vdb_output";
	}


	/// <summary>Retrieve the engine version we're going to expect</summary>
	extern "C" VDBOUTPUTPLUGIN_API int getEngineVersion() {
		return 1;
	}

	/// <summary>Register the plugin to an engine kernel</summary>
	/// <param name="kernel">Kernel the plugin will register to</summary>
	extern "C" VDBOUTPUTPLUGIN_API void registerPlugin(Kernel &kernel) {
		kernel.getOutputMesh().addOutputMeshDriver("vdb",
				auto_ptr<OutputMesh::OutputMeshDriver>(new VDBOutputMeshDriver())
				);
	}

}
