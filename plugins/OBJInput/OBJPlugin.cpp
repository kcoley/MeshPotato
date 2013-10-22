///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#define OBJPLUGIN_SOURCE 1

#include "Config.h"
#include "../MyEngine/Kernel.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
//#include "../MyEngine/Utilities.h"

using namespace std;
namespace MyEngine {

	/// OpenGL graphics drver
	//  class OpenGLGraphicsDriver : public GraphicsServer::GraphicsDriver {
	class OBJInputMeshDriver : public InputMesh::InputMeshDriver {

		/// <summary>Destroys an OpenGL graphics driver</summary>
		public: OBJPLUGIN_API virtual ~OBJInputMeshDriver() {}

			/// <summary>Gets the name of the graphics driver</summary>
		public: OBJPLUGIN_API virtual const std::string &getName() const {
				static string sName("OBJ Mesh Driver");
				return sName;
			}

			/// <summary>Gets the name of the graphics driver</summary>
		public: OBJPLUGIN_API virtual void loadMesh(const char *meshName) {
				static string sName("Loading OBJ");
				std::cout << "Loading OBJ " << meshName << std::endl;
				std::ifstream File(meshName);
				std::string Line;
				std::string Name;
				while(std::getline(File, Line)){
					if(Line == "" || Line[0] == '#')// Skip everything and continue with the next line
						continue;

					std::istringstream LineStream(Line);
					LineStream >> Name;

					if(Name == "v"){// Vertex
						vertex v;
						sscanf(Line.c_str(), "%*s %f %f %f", &v.x, &v.y, &v.z);
						vertices.push_back(v);
					}
					else if(Name == "vn"){// Vertex Normal
						vertex n;
						sscanf(Line.c_str(), "%*s %f %f %f", &n.x, &n.y, &n.z);
						normals.push_back(n);
					}
					else if(Name == "f"){// Vertex Normal
						vertex f;
						sscanf(Line.c_str(), "%*s %f %f %f", &f.x, &f.y, &f.z);
						faces.push_back(f);
					}
				};


			}

		public: OBJPLUGIN_API virtual const size_t getNumberVertices() const{
			return vertices.size();
		}
		public: OBJPLUGIN_API virtual const size_t getNumberNormals() const{
			return normals.size();
		}
		public: OBJPLUGIN_API virtual const size_t getNumberFaces() const{
			return faces.size();
		}
			/// <summary>Creates a renderer</summary>
			//    auto_ptr<Mesh> createRenderer() {
			auto_ptr<Mesh> createInputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return auto_ptr<Mesh>(new Mesh());
			}
		private:


			list<vertex> vertices;
			list<vertex> normals;
			list<vertex> faces;
			//Implement List here for dynamically growing amount of vertices
	};

	/// <summary>Retrieve the engine version we're going to expect</summary>
	extern "C" OBJPLUGIN_API int getEngineVersion() {
		return 1;
	}

	/// <summary>Register the plugin to an engine kernel</summary>
	/// <param name="kernel">Kernel the plugin will register to</summary>
	extern "C" OBJPLUGIN_API void registerPlugin(Kernel &kernel) {
		//    kernel.getGraphicsServer().addGraphicsDriver(
		kernel.getInputMesh().addInputMeshDriver(
				//      auto_ptr<GraphicsServer::GraphicsDriver>(new OpenGLGraphicsDriver())
				auto_ptr<InputMesh::InputMeshDriver>(new OBJInputMeshDriver())
				);
	}

	} // namespace MyEngine
