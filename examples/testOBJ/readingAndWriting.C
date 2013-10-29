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
#include <MeshPotato/meshpotato.h>
#include <openvdb/openvdb.h>
#include <openvdb/io/Stream.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <string>
#include "CmdLineFind.h"

// Alembic Includes
#include <Alembic/AbcGeom/All.h>
// This is required to tell Alembic which implementation to use.  In this case,
// the HDF5 implementation, currently the only one available.
#include <Alembic/AbcCoreHDF5/All.h>
#include <assert.h>
//#include <Alembic/Abc/ErrorHandler.h>
using namespace std;
using namespace MyEngine;
class SimData {
public:
static SimData* getInstance() {
if (!instance)
	instance = new SimData();
return instance;
}
float getVoxelSize() {return voxelSize;}
~SimData() {
delete [] file1;
delete [] file2;
delete instance;
};
private:
SimData() : globalVertices(), globalNormals(), globalFaces(), file1(NULL), file2(NULL), voxelSize(0.05), exBandWidth(3), inBandWidth(3) {}
std::list<std::vector<std::string> > globalVertices;
std::list<std::vector<std::string> > globalNormals;
std::list<std::vector<std::string> > globalFaces;
const char *file1;
const char *file2;
float voxelSize;
float exBandWidth;
float inBandWidth;
static SimData* instance;
};

std::list<std::vector<std::string> > globalVertices;
std::list<std::vector<std::string> > globalNormals;
std::list<std::vector<std::string> > globalFaces;

const char *file1;
const char *file2;
float voxelSize = 0.005;
float exBandWidth = 3;
float inBandWidth = 3;
/// <summary>Example method that tries to open a zip archive</summary>
/// <param name="storageServer">
///   Storage server that will be used to open the zip archive
/// </param>
void listInputMeshDrivers(InputMesh &inputMesh) {
	std::cout << "test1" << std::endl;
//   std::string index = "vdb";
    std::string file(file1);
   std::string index = file.substr(file.find(".")+1, file.length());
//  for(size_t nindex = 0; nindex < inputMesh.getDriverCount(); ++nindex) {
   if (inputMesh.getDriverCount() > 0) {
    cout << "\t" << "name = " << inputMesh.getDriver(index).getName() << endl;
    inputMesh.getDriver(index).loadMesh(file1);
    cout << "Num Vertices = " << inputMesh.getDriver(index).getNumberVertices() << endl;
    cout << "Num Normals = " << inputMesh.getDriver(index).getNumberNormals() << endl;
    cout << "Num Faces = " << inputMesh.getDriver(index).getNumberFaces() << endl;
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
  MeshPotato::MeshSpec spec;
  spec.voxelSize = voxelSize;
  spec.exBandWidth = exBandWidth;
  spec.inBandWidth = inBandWidth;
//  std::string index = "vdb";
    std::string file(file2);
   std::string index = file.substr(file.find(".")+1, file.length());
	std::cout << "output index = " << index << std::endl;
//  for(size_t nindex = 0; nindex < outputMesh.getDriverCount(); ++nindex) {
   if (outputMesh.getDriverCount() > 0) {
    std::cout << "getting output driver..." << std::endl;
    cout << "\t" << outputMesh.getDriver(index).getName() << endl;
    std::cout << "done getting output driver..." << std::endl;
    outputMesh.getDriver(index).loadMesh( globalVertices, globalNormals, globalFaces, spec);
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
  lux::CmdLineFind clf(argc, argv);
  file1 = clf.find("-i", "mesh.obj", "Name of source mesh").c_str();
  file2 = clf.find("-o", "newmesh.obj", "Name of resulting mesh").c_str();
  voxelSize = clf.find("-voxelsize", 0.05f, "Voxel Size (vdb)");
  exBandWidth = clf.find("-exbandwidth", 3, "External Bandwidth (vdb)");
  inBandWidth = clf.find("-inbandwidth", 3, "Internal Bandwidth (vdb)");
  if (argc < 3) {
     clf.usage("-h");
     std::cout << "Invalid number of args" << std::endl;
     exit(0);
 } 
  clf.printFinds(); 
  Kernel theKernel;

  try {
    // Try to open the archive without the required plugins
    cout << "Available input mesh drivers:" << endl;
    listInputMeshDrivers(theKernel.getInputMesh());
    cout << "Available output mesh drivers:" << endl;
    listOutputMeshDrivers(theKernel.getOutputMesh());


    cout << string(79, '*') << endl;
    cout << endl;

    // Now load the plugins
    cout << "Loading plugins..." << endl;
    theKernel.loadPlugin("ABCInputPlugin");
    theKernel.loadPlugin("OBJInputPlugin");
    theKernel.loadPlugin("VDBInputPlugin");
    theKernel.loadPlugin("VDBOutputPlugin");
    theKernel.loadPlugin("OBJOutputPlugin");
//    theKernel.loadPlugin("ABCOutputPlugin");
    cout << endl;

    cout << string(79, '*') << endl;
    cout << endl;

    // Try the same again, this time the plugins are loaded
    cout << "Available mesh drivers:" << endl;
    listInputMeshDrivers(theKernel.getInputMesh());
    cout << "Available output mesh drivers:" << endl;
    listOutputMeshDrivers(theKernel.getOutputMesh());

  }
  catch(const exception &exception) {
    cout << "Unhandled error: " << exception.what() << endl;
  }
}
