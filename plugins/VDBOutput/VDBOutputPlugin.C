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
namespace MeshPotato {
	namespace MPPlugins {

		/// <summary>Destroys an OpenGL graphics driver</summary>
		VDBOutputMesh:: ~VDBOutputMesh() {}

		/// <summary>Gets the name of the graphics driver</summary>
		const char* VDBOutputMesh:: getName() const {
			return "VDB Output Mesh";
		}

		/// <summary>Gets the name of the graphics driver</summary>
		bool VDBOutputMesh::loadMesh(const list<std::vector<std::string> > &vertices, const list<std::vector<std::string> > &normals, const list<std::vector<std::string> > &faces, const MeshPotato::MeshSpec &spec) {
			std::cout <<"Loading VDB Output Mesh"<< std::endl;
			this->vertices.clear();
			this->normals.clear();
			this->faces.clear();
			this->vertices = vertices;
			this->normals = normals;
			this->faces = faces;
			voxelSize = spec.voxelSize;
			exBandWidth = spec.exBandWidth;
			inBandWidth = spec.inBandWidth;
			grid = openvdb::FloatGrid::create(/*background value */2.0);

			return true;
		}

		bool VDBOutputMesh::writeMesh(const char *meshName) {
			std::stringstream strm;
			MeshPotato::MPUtils::Verts verts;
			MeshPotato::MPUtils::Polys polys;
			// Open an OBJ file for writing
			// Write metadata
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


			openvdb::math::Transform::Ptr transform = openvdb::math::Transform::createLinearTransform(voxelSize);



			//					#pragma omp parallel for
			for( size_t i = 0; i < verts.size(); ++i ){
				verts[i] = transform->worldToIndex(verts[i]);
			}

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

		void VDBOutputMesh::setVoxelSize(float voxelSize) {
			this->voxelSize = voxelSize;
		}
		const unsigned int VDBOutputMesh::getNumberVertices() const{
			return vertices.size();
		}
		const unsigned int VDBOutputMesh::getNumberNormals() const{
			return normals.size();
		}
		const unsigned int VDBOutputMesh::getNumberFaces() const{
			return faces.size();
		}

	} // namespace MPPlugins
} // namespace MeshPotato
