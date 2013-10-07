///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
//#define VDBOUTPUTPLUGIN_SOURCE 1

#include "VDBOutputPlugin.h"
//#include <openvdb/openvdb.h>
//#include <openvdb/tools/MeshToVolume.h>
//#include "../MyEngine/Utilities.h"
using namespace std;
namespace MyEngine {

	/// OpenGL graphics drver
	//  class OpenGLGraphicsDriver : public GraphicsServer::GraphicsDriver {
//	class VDBOutputMeshDriver : public OutputMesh::OutputMeshDriver {

		/// <summary>Destroys an OpenGL graphics driver</summary>
		VDBOUTPUTPLUGIN_API VDBOutputMeshDriver:: ~VDBOutputMeshDriver() {}

			/// <summary>Gets the name of the graphics driver</summary>
		VDBOUTPUTPLUGIN_API const std::string& VDBOutputMeshDriver:: getName() const {
				static string sName("VDB Mesh Driver");
				return sName;
			}

			/// <summary>Gets the name of the graphics driver</summary>
		VDBOUTPUTPLUGIN_API void VDBOutputMeshDriver::loadMesh(list<vertex> &vertices, list<vertex> &normals, list<vertex> &faces) {
				this->vertices = vertices;
				this->normals = normals;
				this->faces = faces;
				voxelSize = 0.001;
				grid = openvdb::FloatGrid::create(/*background value */2.0);

				/*
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
				 */

			}

		VDBOUTPUTPLUGIN_API bool VDBOutputMeshDriver::writeMesh(const char *meshName) {
				MeshPotato::MPUtils::Verts verts;
				MeshPotato::MPUtils::Polys polys;
				// Open an OBJ file for writing
					// Write metadata
				std::cout << "Writing vdb mesh" << std::endl;
					// Write vertices
					std::cout << "Pushing vertices" << std::endl;
					for (std::list<vertex>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter) {
						verts.push_back(openvdb::math::Vec3s(iter->x, iter->y, iter->z));
					}

					// Write faces
					std::cout << "Pushing faces" << std::endl;
					for (std::list<vertex>::iterator iter = faces.begin(); iter != faces.end(); ++iter) {
						polys.push_back(openvdb::math::Vec4<uint32_t>((unsigned int)iter->x - 1, (unsigned int)iter->y - 1, (unsigned int)iter->z - 1, openvdb::util::INVALID_IDX));

					}
				

					std::cout << "Transforming voxelSize" << std::endl;
					openvdb::math::Transform::Ptr transform = openvdb::math::Transform::createLinearTransform(voxelSize);


					
//					#pragma omp parallel for
					std::cout << "Converting verts to index space" << std::endl;
				        for( size_t i = 0; i < verts.size(); ++i ){
                				verts[i] = transform->worldToIndex(verts[i]);
        				}

					std::cout << "after transfom to worldness" << std::endl;
					openvdb::tools::MeshToVolume<openvdb::FloatGrid> levelset(transform);
					levelset.convertToLevelSet(verts, polys);
					// Export Mesh
					openvdb::io::File file(meshName);
					grid = levelset.distGridPtr();
					openvdb::GridPtrVec grids;
					grids.push_back(grid);
					file.write(grids);
					file.close();


				return true;
			}

		VDBOUTPUTPLUGIN_API void VDBOutputMeshDriver::setVoxelSize(float voxelSize) {
			  this->voxelSize = voxelSize;
			}
		VDBOUTPUTPLUGIN_API const size_t VDBOutputMeshDriver::getNumberVertices() const{
				return vertices.size();
			}
		VDBOUTPUTPLUGIN_API const size_t VDBOutputMeshDriver::getNumberNormals() const{
				return normals.size();
			}
		VDBOUTPUTPLUGIN_API const size_t VDBOutputMeshDriver::getNumberFaces() const{
				return faces.size();
			}
			/// <summary>Creates a renderer</summary>
			//    auto_ptr<Mesh> createRenderer() {
			auto_ptr<Mesh> VDBOutputMeshDriver::createOutputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return auto_ptr<Mesh>(new Mesh());
			}
//		private:

//			float voxelSize;
//			list<vertex> vertices;
//			list<vertex> normals;
//			list<vertex> faces;
//			openvdb::FloatGrid::Ptr grid;

			//Implement List here for dynamically growing amount of vertices
//	};


	/// <summary>Register the plugin to an engine kernel</summary>
	/// <param name="kernel">Kernel the plugin will register to</summary>
//	extern "C" VDBOUTPUTPLUGIN_API void registerPlugin(Kernel &kernel) {
		//    kernel.getGraphicsServer().addGraphicsDriver(
//		kernel.getOutputMesh().addOutputMeshDriver(
				//      auto_ptr<GraphicsServer::GraphicsDriver>(new OpenGLGraphicsDriver())
//				auto_ptr<OutputMesh::OutputMeshDriver>(new VDBOutputMeshDriver())
//				);
//	}

	} // namespace MyEngine
