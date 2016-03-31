#include <MeshPotato/MPPlugins/pluginapi.h>
#include <MeshPotato/MPUtils/Vector.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
using namespace std;
using namespace MeshPotato::MPPlugins;
namespace MeshPotato {
	namespace MPMesh {
		class VDBOutputMesh : public OutputMesh {
			public:
				virtual ~VDBOutputMesh() {}
				virtual const char *getName() const { return "VDB Output Mesh";}
				virtual bool loadMesh(const list<std::vector<std::string> > &vertices, const list<std::vector<std::string> >&normals, const list<std::vector<std::string> > &faces, const MPUtils::AttributeTable &table)
				{
						mTable.addDoubleAttr("inBandWidth", 3.0);
						mTable.addDoubleAttr("voxelSize", 1.0);
						mTable.addDoubleAttr("exBandWidth", 3.0);
						mTable.addDoubleAttr("inBandWidth", 3.0);
						mTable.addDoubleAttr("backgroundValue", 2.0);
						mTable.mergeTable(table);
					std::cout <<"Loading VDB Output Mesh"<< std::endl;
					this->vertices.clear();
					this->normals.clear();
					this->faces.clear();
					this->vertices = vertices;
					this->normals = normals;
					this->faces = faces;
					voxelSize = mTable.findDoubleAttr("voxelSize");
					exBandWidth = mTable.findDoubleAttr("exBandWidth");
					inBandWidth = mTable.findDoubleAttr("exBandWidth");
					grid = openvdb::FloatGrid::create(mTable.findDoubleAttr("backgroundValue"));

					return true;

				}
				virtual bool writeMesh(const char *meshName) {
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

			}


			openvdb::math::Transform::Ptr transform = openvdb::math::Transform::createLinearTransform(voxelSize);

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
				virtual const unsigned int getNumberVertices() const {
					return vertices.size();
				}
				virtual const unsigned int getNumberNormals() const {
					return normals.size();
				}
				virtual const unsigned int getNumberFaces() const {
					return faces.size();
				}
				virtual void setVoxelSize(float ) {
					this->voxelSize = voxelSize;
				}

			private:
				MPUtils::AttributeTable mTable;
				float voxelSize;
				float exBandWidth;
				float inBandWidth;
				list<std::vector<std::string> > vertices;
				list<std::vector<std::string> > normals;
				list<std::vector<std::string> > faces;
				openvdb::FloatGrid::Ptr grid;

		};
		PLUGIN_FUNC OutputMesh *CreateOutputMesh () {
			return new VDBOutputMesh;
		}
		PLUGIN_FUNC void DestroyOutputMesh(OutputMesh *om) {
			delete om;
		}
		PLUGIN_DISPLAY_NAME("VDB OutputMesh");
		PLUGIN_INIT() {
			std::cout << "PLUGIN_INIT_VDB_OUT" << std::endl;
			RegisterOutputMesh("vdb", CreateOutputMesh, DestroyOutputMesh);
			return 0;
		}

	}

}
