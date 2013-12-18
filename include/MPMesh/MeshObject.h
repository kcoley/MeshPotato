#include <MPPlugins/Kernel.h>
#include <iostream>
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
