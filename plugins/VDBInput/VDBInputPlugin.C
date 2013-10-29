///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
#define VDBINPUTPLUGIN_SOURCE 1

#include "Config.h"
#include <MPPlugins/Kernel.h>
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
namespace MyEngine {


	/// OpenGL graphics drver
	//  class OpenGLGraphicsDriver : public GraphicsServer::GraphicsDriver {
	class VDBInputMeshDriver : public InputMesh::InputMeshDriver {

		/// <summary>Destroys an OpenGL graphics driver</summary>
		public: VDBINPUTPLUGIN_API virtual ~VDBInputMeshDriver() {}

			/// <summary>Gets the name of the graphics driver</summary>
		public: VDBINPUTPLUGIN_API virtual const std::string &getName() const {
				static string sName("VDB Input Mesh Driver");
				return sName;
			}

			/// <summary>Gets the name of the graphics driver</summary>
		public: VDBINPUTPLUGIN_API virtual void loadMesh(const char *meshName) {
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
			openvdb::io::File file2("testingGrid.vdb");
        openvdb::GridPtrVec gridsToWrite;
        gridsToWrite.push_back(baseGrid);
        file2.write(gridsToWrite);
        file2.close();
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

			}

		public: VDBINPUTPLUGIN_API virtual const size_t getNumberVertices() const{
			return vertices.size();
		}
		public: VDBINPUTPLUGIN_API virtual const size_t getNumberNormals() const{
			return normals.size();
		}
		public: VDBINPUTPLUGIN_API virtual const size_t getNumberFaces() const{
			return faces.size();
		}
		public: VDBINPUTPLUGIN_API virtual const std::list<std::vector<std::string> > getVertices() const{
			return vertices;
		}
		public: VDBINPUTPLUGIN_API virtual const std::list<std::vector<std::string> > getNormals() const{
			return normals;
		}
		public: VDBINPUTPLUGIN_API virtual const std::list<std::vector<std::string> > getFaces() const{
			return faces;
		}
			/// <summary>Creates a renderer</summary>
			//    auto_ptr<Mesh> createRenderer() {
			auto_ptr<MPMesh> createInputMesh() {
				//      return auto_ptr<Renderer>(new Renderer());
				return auto_ptr<MPMesh>(new MPMesh());
			}
		private:


			list<std::vector<std::string> > vertices;
			list<std::vector<std::string> > normals;
			list<std::vector<std::string> > faces;
			//Implement List here for dynamically growing amount of vertices
	};

/// <summary>Retrieve the file extension we're going to expect</summary>
        extern "C" VDBINPUTPLUGIN_API const std::string getExtension() {
                return "vdb";
        }

	/// <summary>Retrieve the engine version we're going to expect</summary>
	extern "C" VDBINPUTPLUGIN_API int getEngineVersion() {
		return 1;
	}

	/// <summary>Register the plugin to an engine kernel</summary>
	/// <param name="kernel">Kernel the plugin will register to</summary>
	extern "C" VDBINPUTPLUGIN_API void registerPlugin(Kernel &kernel) {
		//    kernel.getGraphicsServer().addGraphicsDriver(
		kernel.getInputMesh().addInputMeshDriver("vdb",
				//      auto_ptr<GraphicsServer::GraphicsDriver>(new OpenGLGraphicsDriver())
				auto_ptr<InputMesh::InputMeshDriver>(new VDBInputMeshDriver())
				);
	}

	} // namespace MyEngine
