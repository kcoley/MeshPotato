///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#include <MPUtils/CmdLineFind.h>
#include <MPUtils/LuxVector.h>
#include <MPUtils/Vector.h>
#include <MPPlugins/coreapi.h>
#include <MPPlugins/pluginmanager.h>
#include <MeshPotato/meshpotato.h>
#include <boost/filesystem.hpp>
int main(int argc, char **argv) {
  MeshPotato::MPUtils::CmdLineFind clf(argc, argv);
  std::string file1 = clf.find(std::string("-i"), std::string("mesh.obj"), std::string("Name of source mesh"));
  std::string file2 = clf.find(std::string("-o"), std::string("newmesh.obj"), std::string("Name of resulting mesh"));

  std::vector<MeshPotato::MPPlugins::PluginInstance *> plugins =  MeshPotato::MPPlugins::PluginManager::GetInstance().GetAllPlugins();

for (std::vector<MeshPotato::MPPlugins::PluginInstance*>::iterator iter = plugins.begin(); iter != plugins.end(); ++iter) {
	MeshPotato::MPPlugins::PluginInstance *pi = *iter;
	std::cout << "Plugin: " << pi->GetDisplayName() << std::endl;
}

  MeshPotato::MPPlugins::PluginManager::GetInstance().LoadAll();
  float voxelSize = clf.find("-voxelsize", 0.05f, "Voxel Size (vdb)");
  float exBandWidth = clf.find("-exbandwidth", 3, "External Bandwidth (vdb)");
  float inBandWidth = clf.find("-inbandwidth", 3, "Internal Bandwidth (vdb)");
  if (argc < 3) {
     clf.usage("-h");
     std::cout << "Invalid number of args" << std::endl;
     exit(0);
 } 
std::string inputExtension = boost::filesystem::extension(file1);
std::string outputExtension = boost::filesystem::extension(file2);
outputExtension = outputExtension.erase(0,1);
inputExtension = inputExtension.erase(0,1);
MeshPotato::MPMesh::OutputMesh *omesh = MeshPotato::MPPlugins::OutputMeshFactory::CreateOutputMesh(outputExtension);
  if (! omesh) {
	std::cout << "Failed to register outputmesh!" << std::endl;
	return 1;
 }
  MeshPotato::MPMesh::InputMesh *imesh = MeshPotato::MPPlugins::InputMeshFactory::CreateInputMesh(inputExtension);
  if (! imesh) {
	std::cout << "Failed to register inputmesh!" << std::endl;
	return 1;
 }
  imesh->loadMesh(file1.c_str());
  MeshPotato::MPUtils::AttributeTable table;
  table.addDoubleAttr("voxelSize", voxelSize);
  table.addDoubleAttr("exBandWidth", exBandWidth);
  table.addDoubleAttr("inBandWidth", inBandWidth);
  omesh->loadMesh(imesh->getVertices(), imesh->getNormals(), imesh->getFaces(),
   table);	
  omesh->writeMesh(file2.c_str());
  delete omesh; 
  delete imesh; 
  clf.printFinds(); 

}
