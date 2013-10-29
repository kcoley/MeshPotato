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
		VDBOUTPUTPLUGIN_API void VDBOutputMeshDriver::loadMesh(list<std::vector<std::string> > &vertices, list<std::vector<std::string> > &normals, list<std::vector<std::string> > &faces, MeshPotato::MeshSpec spec) {
				this->vertices = vertices;
				this->normals = normals;
				this->faces = faces;
				voxelSize = spec.voxelSize;
				exBandWidth = spec.exBandWidth;
				inBandWidth = spec.inBandWidth;
				grid = openvdb::FloatGrid::create(/*background value */2.0);


			}

		VDBOUTPUTPLUGIN_API bool VDBOutputMeshDriver::writeMesh(const char *meshName) {
				std::stringstream strm;
				MeshPotato::MPUtils::Verts verts;
				MeshPotato::MPUtils::Polys polys;
				// Open an OBJ file for writing
					// Write metadata
				std::cout << "Writing vdb mesh" << std::endl;
					// Write vertices
					std::cout << "Pushing vertices" << std::endl;
					for (std::list<std::vector<std::string> >::iterator iter = vertices.begin(); iter != vertices.end(); ++iter) {
						float x,y,z;
						strm << (*iter)[0];
						strm >> x;
						strm.str("");
						strm.clear();
						strm << (*iter)[1];
						strm >> y;
						strm.str("");
						strm.clear();
						strm << (*iter)[2];
						strm >> z;
						strm.str("");
						strm.clear();
						verts.push_back(openvdb::math::Vec3s(x, y, z));
//						verts.push_back(openvdb::math::Vec3s(iter->x, iter->y, iter->z));
					}

					// Write faces
					std::cout << "Pushing faces" << std::endl;
					for (std::list<std::vector<std::string>  >::iterator iter = faces.begin(); iter != faces.end(); ++iter) {
						unsigned int x,y,z, w;
						strm << (*iter)[0];
						strm >> x;
						strm.str("");
						strm.clear();
						strm << (*iter)[1];
						strm >> y;
						strm.str("");
						strm.clear();
						strm << (*iter)[2];
						strm >> z;
						strm.str("");
						strm.clear();
						if (iter->size() == 3)
							polys.push_back(openvdb::math::Vec4<uint32_t>(x - 1, y - 1, z - 1, openvdb::util::INVALID_IDX));
						else if (iter->size() == 4) {
							strm << (*iter)[3];
							strm >> w;
							strm.str("");
							strm.clear();
							polys.push_back(openvdb::math::Vec4<uint32_t>(x - 1, y - 1, z - 1, w - 1));
							
						}
//						polys.push_back(openvdb::math::Vec4<uint32_t>((unsigned int)iter->x - 1, (unsigned int)iter->y - 1, (unsigned int)iter->z - 1, openvdb::util::INVALID_IDX));

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
					levelset.convertToLevelSet(verts, polys, exBandWidth, inBandWidth);
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
			auto_ptr<MPMesh> VDBOutputMeshDriver::createOutputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return auto_ptr<MPMesh>(new MPMesh());
			}

	} // namespace MyEngine
