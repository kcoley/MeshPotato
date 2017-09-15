///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#define OBJINPUTPLUGIN_SOURCE 1
#include <MeshPotato/MPPlugins/pluginapi.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <MeshPotato/MPUtils/AttributeTable.h>
using namespace MeshPotato::MPPlugins;
namespace MeshPotato {
namespace MPMesh {


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
/*
void keepFaceNormalIndexOnly(const std::string &line, std::vector<std::string>& result) {
std::string item;
std::stringstream ss(line);
                // split up the input string, stripping spaces
                while( std::getline(ss, item, ' ') ){
                        // if item is a face, throw out all other values except 1st (if multiple entries)
                        if( item.find('/') != std::string::npos ){
                                std::stringstream temp(item);
                                std::string r = item.substring(item.find('/'),)
                                std::getline(temp, item, '/');
                        }

                        // if item is not empty, push back
                        if( item.compare("") != 0 && item != "f") result.push_back(item);

                }

}
*/
void getSecondEntry(std::string line, std::string delimiter, std::vector<std::string>& result) {
  std::vector<std::string> tokens;
  std::istringstream iss(line);
  std::copy(
    std::istream_iterator<std::string>(iss),
    std::istream_iterator<std::string>(),
    std::back_inserter(tokens));

    if (tokens.size() > 0 && tokens[0] == "f") {

      for (int i = 0; i < tokens.size(); ++i) {
        size_t pos = 0;
        bool second = false;
        while((pos = tokens[i].find(delimiter)) != std::string::npos) {
            std::string token = tokens[i].substr(0, pos);
            if (second ) {
              //std::cout << token << std::endl;
              result.push_back(token);
              second = ! second;
            }
            else {
              second = ! second;
            }
            tokens[i].erase(0, pos + delimiter.length());
          }
      }
  }
}

void getThirdEntry(std::string line, std::string delimiter, std::vector<std::string>& result) {
  std::vector<std::string> tokens;
  std::istringstream iss(line);
  std::copy(
    std::istream_iterator<std::string>(iss),
    std::istream_iterator<std::string>(),
    std::back_inserter(tokens));

    if (tokens.size() > 0 && tokens[0] == "f") {

      for (int i = 1; i < tokens.size(); ++i) {
        std::vector<std::string> temp_buffer;
        size_t pos = 0;
        bool second = false;
        int count  = 1;
        while((pos = tokens[i].find(delimiter)) != std::string::npos) {
            std::string token = tokens[i].substr(0, pos);
              temp_buffer.push_back(token);

            tokens[i].erase(0, pos + delimiter.length());
          }
          temp_buffer.push_back(tokens[i]);
          result.push_back(temp_buffer[2]);
      }
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
	class OBJInputMesh : public InputMesh {
	public:
		~OBJInputMesh() {}



			/// <summary>Gets the name of the graphics driver</summary>
		const char * getName() const {
				return "OBJ Input Mesh ";
			}

			/// <summary>Gets the name of the graphics driver</summary>
		virtual bool loadMesh(const char *meshName, const MPUtils::AttributeTable &table) {
			vertices.clear();
			normals.clear();
			faces.clear();
			static std::string sName("Loading OBJ");
				std::cout << "Loading OBJ " << meshName << std::endl;
				std::ifstream File(meshName);
				std::string Line;
				std::string Name;
        std::vector<std::vector<std::string> > raw_normals;
        std::vector<std::vector<std::string> > face_normals;
				while(std::getline(File, Line)){
					if(Line == "" || Line[0] == '#')// Skip everything and continue with the next line
						continue;

					std::istringstream LineStream(Line);
					LineStream >> Name;

					if(Name == "v"){// Vertex
						std::vector<std::string> vec = std::vector<std::string>(std::istream_iterator<std::string>(LineStream), std::istream_iterator<std::string>());

						vertices.push_back(vec);
					}
					else if(Name == "vn"){// Vertex Normal
						std::vector<std::string> normal = std::vector<std::string>(std::istream_iterator<std::string>(LineStream), std::istream_iterator<std::string>());
						raw_normals.push_back(normal);
					}
					else if(Name == "f"){// Vertex Normal
						std::vector<std::string> face;
            std::vector<std::string> face_normal;
						keepFaceVertexOnly(Line, face);
            getThirdEntry(Line, "/", face_normal);
						faces.push_back(face);
            face_normals.push_back(face_normal);
					}


				};

        std::cout << "face normal size = " << face_normals.size() << std::endl;
        for (int i = 0; i < face_normals.size(); ++i) {
          std::cout << i << std::endl;
          std::cout << std::stoi(face_normals[i][0])-1 << " " << std::stoi(face_normals[i][1])-1 << " " << std::stoi(face_normals[i][2])-1 << std::endl;
          std::cout << raw_normals[std::stoi(face_normals[i][0])-1][0] << " " << raw_normals[std::stoi(face_normals[i][0])-1][1] << " " << raw_normals[std::stoi(face_normals[i][0])-1][2] << std::endl;
          std::cout << raw_normals[std::stoi(face_normals[i][1])-1][0] << " " << raw_normals[std::stoi(face_normals[i][1])-1][1] << " " << raw_normals[std::stoi(face_normals[i][1])-1][2] << std::endl;
          std::cout << raw_normals[std::stoi(face_normals[i][2])-1][0] << " " << raw_normals[std::stoi(face_normals[i][2])-1][1] << " " << raw_normals[std::stoi(face_normals[i][2])-1][2] << std::endl;

          normals.push_back(raw_normals[std::stoi(face_normals[i][0])-1]);
          normals.push_back(raw_normals[std::stoi(face_normals[i][1])-1]);
          normals.push_back(raw_normals[std::stoi(face_normals[i][2])-1]);
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

	 PLUGIN_FUNC InputMesh *CreateInputMesh() {
                return new OBJInputMesh;
        }

        PLUGIN_FUNC void DestroyInputMesh(InputMesh * om) {
                delete om;
        }

        PLUGIN_DISPLAY_NAME("OBJ InputMesh");
        PLUGIN_INIT() {
                std::cout << "OBJ_INPUT_PLUGIN_INIT" << std::endl;
                RegisterInputMesh("obj", CreateInputMesh, DestroyInputMesh);
                return 0;
        }




	} // namespace MPPlugins
	} // namespace MeshPotato
