///////////////////////////////////////////////////////////////////////////////
// VDBInputPlugin.h                                                          //
//                                                                           //
// Kacey Coley								     //
// This plugin is able to read a VDB levelset and retrieve vertices,         //
// normals and faces.                                                        //
///////////////////////////////////////////////////////////////////////////////

#include <MeshPotato/MPPlugins/pluginapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <openvdb/openvdb.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <MeshPotato/MPUtils/Vector.h>
using namespace std;
using namespace MeshPotato::MPPlugins;
namespace MeshPotato {
	namespace MPMesh {


		class VDBInputMesh : public InputMesh {
			public:
				virtual ~VDBInputMesh() {}

				virtual const char *getName() const {
					return "VDB Input Mesh";
				}

				virtual bool loadMesh(const char *meshName, const MPUtils::AttributeTable &table) {
					openvdb::initialize();
					openvdb::GridBase::Ptr baseGrid;
					openvdb::FloatGrid::Ptr grid;
					openvdb::GridPtrVecPtr grids(new openvdb::GridPtrVec);
					static string sName("Loading VDB ");
					std::cout << sName << meshName << std::endl;
					openvdb::io::File file(meshName);
					file.open();
					openvdb::io::File::NameIterator nameIter = file.beginName();
					baseGrid = file.readGrid(nameIter.gridName());
					std::cout << "#INFO Gridname = " << nameIter.gridName() << std::endl;
					std::cout << "#INFO <ReadMesh> active voxel count: " << baseGrid->activeVoxelCount() << std::endl;
					baseGrid->setGridClass(openvdb::GRID_LEVEL_SET);
					grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);
					openvdb::tools::VolumeToMesh mesher;
					mesher.operator()<openvdb::FloatGrid>(grid.operator*());
					std::cout << "points: " << mesher.pointListSize()
						<< " polys: " << mesher.polygonPoolListSize() << std::endl;

					openvdb::tools::PointList *verts = &mesher.pointList();
					openvdb::tools::PolygonPoolList *polys = &mesher.polygonPoolList();

					for (size_t i = 0; i < mesher.pointListSize(); ++i) {
						std::stringstream strm;
						std::string x,y,z;
						openvdb::Vec3s &v = (*verts)[i];
						std::vector<std::string> vec;
						vec.reserve(3);
						strm << v[0];
						strm >> x;
						strm.str("");
						strm.clear();
						strm << v[1];
						strm >> y;
						strm.str("");
						strm.clear();

						strm << v[2];
						strm >> z;
						strm.str("");
						strm.clear();
						vec.push_back(x);
						vec.push_back(y);
						vec.push_back(z);
						vertices.push_back(vec);
					}

					for( size_t i = 0; i < mesher.polygonPoolListSize(); ++i ){

						for( size_t ndx = 0; ndx < (*polys)[i].numTriangles(); ndx++ ){
							std::stringstream strm;
							std::string x,y,z;
							openvdb::Vec3I *p = &((*polys)[i].triangle(ndx));
							std::vector<std::string> face_vec;
							strm << p->x() + 1;
							strm >> x;
							strm.str("");
							strm.clear();
							strm << p->y() + 1;
							strm >> y;
							strm.str("");
							strm.clear();

							strm << p->z() + 1;
							strm >> z;
							strm.str("");
							strm.clear();
							face_vec.push_back(x);
							face_vec.push_back(y);
							face_vec.push_back(z);
							faces.push_back(face_vec);

						}

						for( size_t ndx = 0; ndx < (*polys)[i].numQuads(); ndx++ ){
							std::stringstream strm;
							std::string x,y,z,w;
							openvdb::Vec4I *p = &((*polys)[i].quad(ndx));
							std::vector<std::string> face_vec;
							strm << p->x() + 1;
							strm >> x;
							strm.str("");
							strm.clear();
							strm << p->y() + 1;
							strm >> y;
							strm.str("");
							strm.clear();

							strm << p->z() + 1;
							strm >> z;
							strm.str("");
							strm.clear();
							strm << p->w() + 1;
							strm >> w;
							strm.str("");
							strm.clear();
							face_vec.push_back(x);
							face_vec.push_back(y);
							face_vec.push_back(z);
							face_vec.push_back(w);
							faces.push_back(face_vec);
						}



					}
					std::cout << "points: " << mesher.polygonPoolListSize()
						<< " polys: " << mesher.polygonPoolListSize() << std::endl;

					std::cout << "face size = " << faces.size() << std::endl;

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
		};

		PLUGIN_FUNC InputMesh *CreateInputMesh() {
			return new VDBInputMesh;
		}
		PLUGIN_FUNC void DestroyInputMesh(InputMesh *om) {
			delete om;
		}
		PLUGIN_DISPLAY_NAME("VDB InputMesh");
		PLUGIN_INIT() {
			std::cout << "PLUGIN_INIT_VDB_INPUT" << std::endl;
			RegisterInputMesh("vdb", CreateInputMesh, DestroyInputMesh);
			return 0;
		}

	}// namespace MPMesh

} // namespace MeshPotato
