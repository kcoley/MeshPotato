///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
//#define OBJOUTPUTPLUGIN_SOURCE 1

#include <MPPlugins/pluginapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <vector>
#include <MeshPotato/meshpotato.h>
using namespace MeshPotato::MPPlugins;
namespace MeshPotato {
namespace MPMesh {
	class OBJOutputMesh : public OutputMesh {
	public:
	~OBJOutputMesh() {}

			/// <summary>Gets the name of the graphics driver</summary>
	virtual const char* getName() const {
				return "OBJOutputMesh";
	}

			/// <summary>Gets the name of the graphics driver</summary>
	virtual bool loadMesh(const std::list<std::vector<std::string> > &vertices, const std::list<std::vector<std::string> > &normals, const std::list<std::vector<std::string> > &faces, const MeshPotato::MPUtils::AttributeTable  &table) {
				this->vertices = vertices;
				this->normals = normals;
				this->faces = faces;
				return true;
			}
	virtual bool writeMesh(const char *meshName) {
				// Open an OBJ file for writing
				std::ofstream objFile;
				objFile.open(meshName, std::ios::out);
				if (objFile.is_open()) {
					// Write metadata

					// Write vertices
					for (std::list<std::vector<std::string> >::iterator iter = vertices.begin(); iter != vertices.end(); ++iter) {
						objFile << "v ";
						for (std::vector<std::string>::iterator ptr = iter->begin(); ptr != iter->end(); ++ptr) {
						 	objFile << *ptr << " ";
						}
						objFile << std::endl;
					}
					objFile << std::endl;
					// Write normals
					for (std::list<std::vector<std::string> >::iterator iter = normals.begin(); iter != normals.end(); ++iter) {
						objFile << "vn ";
						for (std::vector<std::string>::iterator ptr = iter->begin(); ptr != iter->end(); ++ptr) {
						 	objFile << *ptr << " ";
						}
						objFile << std::endl;
					}
					objFile << std::endl;

					// Write faces
					for (std::list<std::vector<std::string> >::iterator iter = faces.begin(); iter != faces.end(); ++iter) {
						objFile << "f ";
						for (std::vector<std::string>::iterator ptr = iter->begin(); ptr != iter->end(); ++ptr) {
						 	objFile << *ptr << " ";
						}
						objFile << std::endl;
					}
					objFile << std::endl;
				}
				else {
					return false;
				}

				return true;
			}

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
	};

	PLUGIN_FUNC OutputMesh *CreateOutputMesh() {
		return new OBJOutputMesh;
	}

	PLUGIN_FUNC void DestroyOutputMesh(OutputMesh * om) {
		delete om;
	}

	PLUGIN_DISPLAY_NAME("OBJ OutputMesh");
	PLUGIN_INIT() {
		std::cout << "OBJ_OUTPUT_PLUGIN_INIT" << std::endl;
		RegisterOutputMesh("obj", CreateOutputMesh, DestroyOutputMesh);
		return 0;
	}
} // namespace MyPlugins
} // namespace MeshPotato
