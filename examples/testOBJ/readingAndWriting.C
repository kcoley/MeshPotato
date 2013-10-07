///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#include <MPPlugins/Kernel.h>
#include <MPUtils/Vector.h>
#include <iostream>
#include <stdexcept>

#include <openvdb/openvdb.h>
#include <openvdb/io/Stream.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <string>


using namespace std;
using namespace MyEngine;

std::list<vertex> globalVertices;
std::list<vertex> globalNormals;
std::list<vertex> globalFaces;

char *file1;
char *file2;
static float voxelSize = 0.5;
/// <summary>Example method that tries to open a zip archive</summary>
/// <param name="storageServer">
///   Storage server that will be used to open the zip archive
/// </param>
void tryOpenArchive(StorageServer &storageServer) {
  try {
    // This method will succeed only if the zip plugin is registered
    // to the StorageServer
    storageServer.openArchive("some_archive.zip");

    cout << "\tArchive successfully opened" << endl;
  }
  catch(const exception &exception) {
    cout << "\tError opening archive: " << exception.what() << endl;
  }

  cout << endl;
}

/// <summary>Example method that lists available graphics drivers</summary>
/// <param name="graphicsServer">
///   Storage server that will be used to open the zip archive
/// </param>
void listGraphicsDrivers(GraphicsServer &graphicsServer) {
  for(size_t index = 0; index < graphicsServer.getDriverCount(); ++index) {
    cout << "\t" << graphicsServer.getDriver(index).getName() << endl;
  }

  if(graphicsServer.getDriverCount() == 0) {
    cout << "\t" << "None" << endl;
  }

  cout << endl;
}

void listInputMeshDrivers(InputMesh &inputMesh) {
	std::cout << "test1" << std::endl;
//   std::string index = "vdb";
    std::string file(file1);
   std::string index = file.substr(file.find(".")+1, file.length());
  std::cout << "extension = " << index << std::endl;
//  for(size_t nindex = 0; nindex < inputMesh.getDriverCount(); ++nindex) {
   if (inputMesh.getDriverCount() > 0) {
    cout << "\t" << "name = " << inputMesh.getDriver(index).getName() << endl;
	std::cout << "test2" << std::endl;
	std::cout << "file1 = " << file1 << std::endl;
    inputMesh.getDriver(index).loadMesh(file1);
	std::cout << "test2.5" << std::endl;
    cout << "Num Vertices = " << inputMesh.getDriver(index).getNumberVertices() << endl;
    cout << "Num Normals = " << inputMesh.getDriver(index).getNumberNormals() << endl;
    cout << "Num Faces = " << inputMesh.getDriver(index).getNumberFaces() << endl;
	std::cout << "test3" << std::endl;
    globalVertices = inputMesh.getDriver(index).getVertices(); 
    globalNormals = inputMesh.getDriver(index).getNormals(); 
    globalFaces = inputMesh.getDriver(index).getFaces(); 
 }

  if(inputMesh.getDriverCount() == 0) {
    cout << "\t" << "None" << endl;
  }

  cout << endl;
}
void listOutputMeshDrivers(OutputMesh &outputMesh) {
  list<vertex> vertices, faces, normals;
  vertices.push_back(vertex(1,1,1));
  normals.push_back(vertex(1,1,1));
  faces.push_back(vertex(1,1,1));
//  std::string index = "vdb";
    std::string file(file2);
   std::string index = file.substr(file.find(".")+1, file.length());
	std::cout << "output index = " << index << std::endl;
//  for(size_t nindex = 0; nindex < outputMesh.getDriverCount(); ++nindex) {
   if (outputMesh.getDriverCount() > 0) {
    cout << "\t" << outputMesh.getDriver(index).getName() << endl;
    outputMesh.getDriver(index).loadMesh( globalVertices, globalNormals, globalFaces);
    cout << "Num Vertices = " << outputMesh.getDriver(index).getNumberVertices() << endl;
    cout << "Num Normals = " << outputMesh.getDriver(index).getNumberNormals() << endl;
    cout << "Num Faces = " << outputMesh.getDriver(index).getNumberFaces() << endl;
//    outputMesh.getDriver(index).setVoxelSize(voxelSize);
    std::string filename(file2);
    outputMesh.getDriver(index).writeMesh((filename).c_str());
//    MyEngine::VDBOutputMeshDriver driver;
  }

  if(outputMesh.getDriverCount() == 0) {
    cout << "\t" << "None" << endl;
  }

  cout << endl;
}

/// <summary>Program entry point</summary>
int main(int argc, char **argv) {
  if (argc < 3) {
     std::cout << "Invalid number of args" << std::endl;
  }
  file1 = argv[1];
  file2 = argv[2];
  if (argc == 3) {
     voxelSize = atof(argv[4]);
  }
  
  Kernel theKernel;

  try {
    // Try to open the archive without the required plugins
    cout << "Trying to open archive before plugins were loaded..." << endl;
    tryOpenArchive(theKernel.getStorageServer());
    cout << "Available graphics drivers:" << endl;
    listGraphicsDrivers(theKernel.getGraphicsServer());
    cout << "Available input mesh drivers:" << endl;
    listInputMeshDrivers(theKernel.getInputMesh());
    cout << "Available output mesh drivers:" << endl;
    listOutputMeshDrivers(theKernel.getOutputMesh());


    cout << string(79, '*') << endl;
    cout << endl;

    // Now load the plugins
    cout << "Loading plugins..." << endl;
//    theKernel.loadPlugin("OpenGLPlugin");
//    theKernel.loadPlugin("ZipPlugin");
    theKernel.loadPlugin("OBJInputPlugin");
    theKernel.loadPlugin("VDBInputPlugin");
    theKernel.loadPlugin("OBJOutputPlugin");
    theKernel.loadPlugin("VDBOutputPlugin");
    cout << endl;

    cout << string(79, '*') << endl;
    cout << endl;

    // Try the same again, this time the plugins are loaded
    cout << "Trying to open archive with plugins loaded..." << endl;
    tryOpenArchive(theKernel.getStorageServer());
    cout << "Available graphics drivers:" << endl;
    listGraphicsDrivers(theKernel.getGraphicsServer());
    cout << "Available mesh drivers:" << endl;
    listInputMeshDrivers(theKernel.getInputMesh());
    cout << "Available output mesh drivers:" << endl;
    listOutputMeshDrivers(theKernel.getOutputMesh());

  }
  catch(const exception &exception) {
    cout << "Unhandled error: " << exception.what() << endl;
  }
}
