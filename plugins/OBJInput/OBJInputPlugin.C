///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#define OBJINPUTPLUGIN_SOURCE 1

#include "Config.h"
#include <MPPlugins/Kernel.h>
#include <MPUtils/Vector.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <stdlib.h>
//#include "../MyEngine/Utilities.h"

using namespace std;
namespace MyEngine {


void keepFaceVertexOnly(const std::string &line, std::vector<std::string>& result) {
std::string item;
std::stringstream ss(line);
                // split up the input string, stripping spaces
                while( std::getline(ss, item, ' ') ){
                        // if item is a face, throw out all other values except 1st (if multiple entries)
                        if( item.find('/') != std::string::npos ){
                                std::stringstream temp(item);
                                std::getline(temp, item, '/');
                        }

                        // if item is not empty, push back
                        if( item.compare("") != 0 ) result.push_back(item);

                }

}

void split(const std::string& in, std::vector<std::string>& result){

        size_t pos = 0;
        size_t ndx = in.find(' ');

        while( true ){

                if( in[pos] != ' ' )
                        result.push_back( in.substr(pos, ndx - pos) );

                if( ndx == std::string::npos ) break;
                pos = ndx+1;
                ndx = in.find(' ', pos);
        }
}






	/// OpenGL graphics drver
	//  class OpenGLGraphicsDriver : public GraphicsServer::GraphicsDriver {
	class OBJInputMeshDriver : public InputMesh::InputMeshDriver {

		/// <summary>Destroys an OpenGL graphics driver</summary>
		public: OBJINPUTPLUGIN_API virtual ~OBJInputMeshDriver() {}

			/// <summary>Gets the name of the graphics driver</summary>
		public: OBJINPUTPLUGIN_API virtual const std::string &getName() const {
				static string sName("OBJ Input Mesh Driver");
				return sName;
			}

			/// <summary>Gets the name of the graphics driver</summary>
		public: OBJINPUTPLUGIN_API virtual void loadMesh(const char *meshName) {
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
						std::vector<std::string> vect;
						vertex f;
						keepFaceVertexOnly(Line, vect);
						f.x = ::atof(vect[1].c_str());
						f.y = ::atof(vect[2].c_str());
						f.z = ::atof(vect[3].c_str());
						sscanf(Line.c_str(), "%*s %f %f %f", &f.x, &f.y, &f.z);
						faces.push_back(f);
					}
				};


			}

		public: OBJINPUTPLUGIN_API virtual const size_t getNumberVertices() const{
			return vertices.size();
		}
		public: OBJINPUTPLUGIN_API virtual const size_t getNumberNormals() const{
			return normals.size();
		}
		public: OBJINPUTPLUGIN_API virtual const size_t getNumberFaces() const{
			return faces.size();
		}
		public: OBJINPUTPLUGIN_API virtual const std::list<vertex> getVertices() const{
			return vertices;
		}
		public: OBJINPUTPLUGIN_API virtual const std::list<vertex> getNormals() const{
			return normals;
		}
		public: OBJINPUTPLUGIN_API virtual const std::list<vertex> getFaces() const{
			return faces;
		}
			/// <summary>Creates a renderer</summary>
			//    auto_ptr<Mesh> createRenderer() {
			auto_ptr<MPMesh> createInputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return auto_ptr<MPMesh>(new MPMesh());
			}
		private:


			list<vertex> vertices;
			list<vertex> normals;
			list<vertex> faces;
			std::string file_ext;
			//Implement List here for dynamically growing amount of vertices
	};

	/// <summary>Retrieve the engine version we're going to expect</summary>
	extern "C" OBJINPUTPLUGIN_API int getEngineVersion() {
		return 1;
	}
	/// <summary>Retrieve the file extension we're going to expect</summary>
	extern "C" OBJINPUTPLUGIN_API const std::string getExtension() {
		return "obj";
	}

	/// <summary>Register the plugin to an engine kernel</summary>
	/// <param name="kernel">Kernel the plugin will register to</summary>
	extern "C" OBJINPUTPLUGIN_API void registerPlugin(Kernel &kernel) {
		//    kernel.getGraphicsServer().addGraphicsDriver(
		kernel.getInputMesh().addInputMeshDriver("obj",
				//      auto_ptr<GraphicsServer::GraphicsDriver>(new OpenGLGraphicsDriver())
				auto_ptr<InputMesh::InputMeshDriver>(new OBJInputMeshDriver())
				);
	}

	} // namespace MyEngine
