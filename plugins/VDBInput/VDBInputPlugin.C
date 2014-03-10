///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////

#include <MPPlugins/pluginapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <openvdb/openvdb.h>
//#include <openvdb/io/Stream.h>
#include <openvdb/tools/VolumeToMesh.h>
//#include "../MyEngine/Utilities.h"
#include <MPUtils/Vector.h>
using namespace std;
namespace MeshPotato {
	namespace MPPlugins {


	/// OpenGL graphics drver
	//  class OpenGLGraphicsDriver : public GraphicsServer::GraphicsDriver {
	class VDBInputMesh : public InputMeshAPI {
	public:
		/// <summary>Destroys an OpenGL graphics driver</summary>
		virtual ~VDBInputMesh() {}

			/// <summary>Gets the name of the graphics driver</summary>
		virtual const char *getName() const {
				return "VDB Input Mesh";
			}

			/// <summary>Gets the name of the graphics driver</summary>
		virtual bool loadMesh(const char *meshName) {
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
			std::cout << "Gridname = " << nameIter.gridName() << std::endl;
			 std::cout << "#INFO <ReadMesh> active voxel count: " << baseGrid->activeVoxelCount() << std::endl;
			// Get the vertices and faces from the levelset
			// Identify the grid as a level set.
		        baseGrid->setGridClass(openvdb::GRID_LEVEL_SET);
//			openvdb::io::File file2("testingGrid.vdb");
///        openvdb::GridPtrVec gridsToWrite;
//        gridsToWrite.push_back(baseGrid);
//        file2.write(gridsToWrite);
//        file2.close();
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
//				vertices.push_back(vertex(v[0], v[1], v[2]));
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
//					faces.push_back(vertex(p->x() + 1, p->y() + 1, p->z() + 1));

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
				
//				faces.push_back(vertex(p->x() + 1, p->y() + 1, p->z() + 1, p->w() + 1));
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
			//Implement List here for dynamically growing amount of vertices
	};

	PLUGIN_FUNC InputMeshAPI *CreateInputMesh() {
		return new VDBInputMesh;
	}
	PLUGIN_FUNC void DestroyInputMesh(InputMeshAPI *om) {
		delete om;
	}
	PLUGIN_DISPLAY_NAME("VDB InputMesh");
	PLUGIN_INIT() {
		std::cout << "PLUGIN_INIT_VDB_INPUT" << std::endl;
		RegisterInputMesh("vdb", CreateInputMesh, DestroyInputMesh);
		return 0;
	}

	}// namespace MPPlugins

	} // namespace MeshPotato
