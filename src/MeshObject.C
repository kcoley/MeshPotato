#include "MPMesh/MeshObject.h"
  
namespace MeshPotato {
  
MeshObject::MeshObject() : theKernel(), inputMesh(), outputMesh() {
   std::cout << "Default constructor" << std::endl;
    theKernel.loadPlugin("OBJInputPlugin");
    theKernel.loadPlugin("VDBInputPlugin");
    theKernel.loadPlugin("VDBOutputPlugin");
    theKernel.loadPlugin("OBJOutputPlugin");
    inputMesh = &theKernel.getInputMesh();
    outputMesh = &theKernel.getOutputMesh();
}
MeshObject::~MeshObject() {
 
}
void MeshObject::loadMesh(std::string &name) {
	std::string index = name.substr(name.find(".")+1, name.length());
	if (inputMesh->getDriverCount() > 0) {
    		std::cout << "\t" << "name = " << inputMesh->getDriver(index).getName() << std::endl;
    		inputMesh->getDriver(index).loadMesh(name.c_str());
    		std::cout << "Num Vertices = " << inputMesh->getDriver(index).getNumberVertices() << std::endl;
    		std::cout << "Num Normals = " << inputMesh->getDriver(index).getNumberNormals() << std::endl;
    		std::cout << "Num Faces = " << inputMesh->getDriver(index).getNumberFaces() << std::endl;
    		globalVertices = inputMesh->getDriver(index).getVertices();
    		globalNormals = inputMesh->getDriver(index).getNormals();
    		globalFaces = inputMesh->getDriver(index).getFaces();
 	}
	if(inputMesh->getDriverCount() == 0) {
    		std::cout << "\t" << "None" << std::endl;
  	}

}
void MeshObject::writeMesh(std::string &name) {
	std::string index = name.substr(name.find(".")+1, name.length());
        std::cout << "output index = " << index << std::endl;
   if (outputMesh->getDriverCount() > 0) {
    std::cout << "getting output driver..." << std::endl;
    std::cout << "\t" << outputMesh->getDriver(index).getName() << std::endl;
    std::cout << "done getting output driver..." << std::endl;
	MeshPotato::MeshSpec spec = MeshPotato::MeshSpec();
    outputMesh->getDriver(index).loadMesh( globalVertices, globalNormals, globalFaces, spec);
    std::cout << "Num Vertices = " << outputMesh->getDriver(index).getNumberVertices() << std::endl;
    std::cout << "Num Normals = " << outputMesh->getDriver(index).getNumberNormals() << std::endl;
    std::cout << "Num Faces = " << outputMesh->getDriver(index).getNumberFaces() << std::endl;
    std::string filename(name);
    outputMesh->getDriver(index).writeMesh((filename).c_str());
  }

  if(outputMesh->getDriverCount() == 0) {
    std::cout << "\t" << "None" << std::endl;
  }

}

}

