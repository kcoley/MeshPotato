#include <MPPlugins/Kernel.h>
#include <iostream>
#include <string>
//#include <MPUtils/Vector.h>
#include <stdexcept>
#include <MeshPotato/meshpotato.h>
#include <openvdb/openvdb.h>
//#include <openvdb/io/Stream.h>
//#include <openvdb/tools/VolumeToMesh.h>
#include <string>


namespace MeshPotato {
class MeshObject {
public:
	MeshObject();
	~MeshObject();
        // Load Mesh
        void loadMesh(std::string &name);
//        void loadMesh(std::string &name, spec);
        void writeMesh(std::string &name);
//        void writeMesh(std::string &name, spec);
        // Write Mesh
	std::list<std::vector<std::string> > getVertices() const {
		return globalVertices;
	}
	std::list<std::vector<std::string> > getNormals() const {
		return globalNormals;
	}
	std::list<std::vector<std::string> > getFaces() const {
		return globalFaces;
	}
	
private:
        //vertex, normals, etc. data
	MyEngine::Kernel theKernel;
	MyEngine::InputMesh *inputMesh;
	MyEngine::OutputMesh *outputMesh;
	std::list<std::vector<std::string> > globalVertices;
	std::list<std::vector<std::string> > globalNormals;
	std::list<std::vector<std::string> > globalFaces;
};
}
