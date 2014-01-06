///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
//#define OBJOUTPUTPLUGIN_SOURCE 1

#include "Config.h"
#include <MPPlugins/Kernel.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
//#include "../MyEngine/Utilities.h"

namespace MyEngine {

	/// OpenGL graphics drver
	//  class OpenGLGraphicsDriver : public GraphicsServer::GraphicsDriver {
	class OBJOutputMeshDriver : public OutputMesh::OutputMeshDriver {
	public:
		/// <summary>Destroys an OpenGL graphics driver</summary>
	OBJOUTPUTPLUGIN_API virtual ~OBJOutputMeshDriver() {}

			/// <summary>Gets the name of the graphics driver</summary>
	OBJOUTPUTPLUGIN_API virtual const std::string &getName() const {
				static std::string sName("OBJ Output Mesh Driver");
				return sName;
	}

			/// <summary>Gets the name of the graphics driver</summary>
	OBJOUTPUTPLUGIN_API virtual void loadMesh(std::list<std::vector<std::string> > &vertices, std::list<std::vector<std::string> > &normals, std::list<std::vector<std::string> > &faces, MeshPotato::MeshSpec &spec) {
				this->vertices = vertices;
				this->normals = normals;
				this->faces = faces;
			}
	OBJOUTPUTPLUGIN_API virtual bool writeMesh(const char *meshName) {
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

	OBJOUTPUTPLUGIN_API virtual const size_t getNumberVertices() const{
				return vertices.size();
			}
	OBJOUTPUTPLUGIN_API virtual const size_t getNumberNormals() const{
				return normals.size();
			}
	OBJOUTPUTPLUGIN_API virtual const size_t getNumberFaces() const{
				return faces.size();
			}


			/// <summary>Creates a renderer</summary>
			//    auto_ptr<Mesh> createRenderer() {
			std::auto_ptr<MPMesh> createOutputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return std::auto_ptr<MPMesh>(new MPMesh());
			}
	private:
			std::list<std::vector<std::string> > vertices;
			std::list<std::vector<std::string> > normals;
			std::list<std::vector<std::string> > faces;
			//Implement List here for dynamically growing amount of vertices
	};

	/// <summary>Retrieve the file extension we're going to expect</summary>
	extern "C" OBJOUTPUTPLUGIN_API const std::string getExtension() {
                return "obj_output";
        }

	/// <summary>Retrieve the engine version we're going to expect</summary>
	extern "C" OBJOUTPUTPLUGIN_API int getEngineVersion() {
		return 1;
	}

	/// <summary>Register the plugin to an engine kernel</summary>
	/// <param name="kernel">Kernel the plugin will register to</summary>
	extern "C" OBJOUTPUTPLUGIN_API void registerPlugin(Kernel &kernel) {
		kernel.getOutputMesh().addOutputMeshDriver("obj",
				std::auto_ptr<OutputMesh::OutputMeshDriver>(new OBJOutputMeshDriver())
				);
	}

} // namespace MyEngine
