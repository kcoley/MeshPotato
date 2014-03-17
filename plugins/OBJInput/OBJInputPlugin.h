///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#define OBJINPUTPLUGIN_SOURCE 1
#include <MPPlugins/pluginapi.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <stdlib.h>

namespace MeshPotato {
namespace MPPlugins {


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
                        if( item.compare("") != 0 && item != "f") result.push_back(item);

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
	class OBJInputMesh : public InputMeshAPI {
	public:
		~OBJInputMesh() {}



			/// <summary>Gets the name of the graphics driver</summary>
		const char * getName() const {
				return "OBJ Input Mesh ";
			}

			/// <summary>Gets the name of the graphics driver</summary>
		virtual bool loadMesh(const char *meshName) {
			vertices.clear();
			normals.clear();
			faces.clear();
			static std::string sName("Loading OBJ");
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
//						vertex v;
						std::vector<std::string> vec = std::vector<std::string>(std::istream_iterator<std::string>(LineStream), std::istream_iterator<std::string>());
//						sscanf(Line.c_str(), "%*s %f %f %f", &v.x, &v.y, &v.z);
						
						vertices.push_back(vec);
					}
					else if(Name == "vn"){// Vertex Normal
//						vertex n;
						std::vector<std::string> normal = std::vector<std::string>(std::istream_iterator<std::string>(LineStream), std::istream_iterator<std::string>());
//						sscanf(Line.c_str(), "%*s %f %f %f", &n.x, &n.y, &n.z);
						normals.push_back(normal);
					}
					else if(Name == "f"){// Vertex Normal
						std::vector<std::string> face;
//						vertex f;
						keepFaceVertexOnly(Line, face);
//						f.x = ::atof(vect[1].c_str());
//						f.y = ::atof(vect[2].c_str());
//						f.z = ::atof(vect[3].c_str());
//						sscanf(Line.c_str(), "%*s %f %f %f", &f.x, &f.y, &f.z);
						faces.push_back(face);
					}
				};
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
		virtual const std::list<std::vector<std::string> > getVertices() const{
			return vertices;
		}
		virtual const std::list<std::vector<std::string> > getNormals() const{
			return normals;
		}
		virtual const std::list<std::vector<std::string> > getFaces() const{
			return faces;
		}
		private:


			std::list<std::vector<std::string> > vertices;
			std::list<std::vector<std::string> > normals;
			std::list<std::vector<std::string> > faces;
			std::string file_ext;
	};

	 PLUGIN_FUNC InputMeshAPI *CreateInputMesh() {
                return new OBJInputMesh;
        }

        PLUGIN_FUNC void DestroyInputMesh(InputMeshAPI * om) {
                delete om;
        }

        PLUGIN_DISPLAY_NAME("OBJ InputMesh");
        PLUGIN_INIT() {
                std::cout << "PLUGIN_INIT" << std::endl;
                RegisterInputMesh("obj", CreateInputMesh, DestroyInputMesh);
                return 0;
        }

	


	} // namespace MPPlugins
	} // namespace MeshPotato
