///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#include <MPPlugins/Kernel.h>

#include <iostream>
#include <stdexcept>

using namespace std;
using namespace MyEngine;

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
  for(size_t index = 0; index < inputMesh.getDriverCount(); ++index) {
    cout << "\t" << inputMesh.getDriver(index).getName() << endl;
    inputMesh.getDriver(index).loadMesh("/home/kcoley/Desktop/pluginTest/pluginMesh/cube.obj");
    cout << "Num Vertices = " << inputMesh.getDriver(index).getNumberVertices() << endl;
    cout << "Num Normals = " << inputMesh.getDriver(index).getNumberNormals() << endl;
    cout << "Num Faces = " << inputMesh.getDriver(index).getNumberFaces() << endl;
  }

  if(inputMesh.getDriverCount() == 0) {
    cout << "\t" << "None" << endl;
  }

  cout << endl;
}
void listOutputMeshDrivers(OutputMesh &outputMesh) {
  for(size_t index = 0; index < outputMesh.getDriverCount(); ++index) {
    cout << "\t" << outputMesh.getDriver(index).getName() << endl;
    outputMesh.getDriver(index).loadMesh("/home/kcoley/Desktop/pluginTest/pluginMesh/cube.obj");
    cout << "Num Vertices = " << outputMesh.getDriver(index).getNumberVertices() << endl;
    cout << "Num Normals = " << outputMesh.getDriver(index).getNumberNormals() << endl;
    cout << "Num Faces = " << outputMesh.getDriver(index).getNumberFaces() << endl;
  }

  if(outputMesh.getDriverCount() == 0) {
    cout << "\t" << "None" << endl;
  }

  cout << endl;
}

/// <summary>Program entry point</summary>
int main() {
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
    theKernel.loadPlugin("OpenGLPlugin");
    theKernel.loadPlugin("ZipPlugin");
    theKernel.loadPlugin("OBJInputPlugin");
    theKernel.loadPlugin("OBJOutputPlugin");
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
