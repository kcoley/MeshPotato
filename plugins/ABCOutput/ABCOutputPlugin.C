///////////////////////////////////////////////////////////////////////////////
// Plugin architecture example                                               //
//                                                                           //
// This code serves as an example to the plugin architecture discussed in    //
// the article and can be freely used.                                       //
///////////////////////////////////////////////////////////////////////////////
//#define ABCOUTPUTPLUGIN_SOURCE 1

#include "ABCOutputPlugin.h"
#include "MeshData.h"
//#include "../MyEngine/Utilities.h"
namespace MeshPotato {
	namespace MPMesh {


	ABCOutputMesh::~ABCOutputMesh() {
		delete [] g_verts;
		delete [] g_indices;
		delete [] g_counts;
	}

	 const char* ABCOutputMesh::getName() const {
				static std::string sName("ABC Output Mesh Driver");
				return sName.c_str();
	}

  bool ABCOutputMesh::loadMesh(const std::list<std::vector<std::string> > &vertices,
				const std::list<std::vector<std::string> > &normals,
				const std::list<std::vector<std::string> > &faces,
				const MeshPotato::MPUtils::AttributeTable &table) {


				int index = 0;
				this->vertices = vertices;
				this->normals = normals;
				this->faces = faces;

				// Convert to alembic friendly versions
				g_numVerts = vertices.size()*3;
				g_verts = new Alembic::Abc::float32_t[g_numVerts];
				//fill...

				for (std::list<std::vector<std::string> >::const_iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
					g_verts[index++] = ::atof(((*itr)[0]).c_str());
					g_verts[index++] = ::atof(((*itr)[1]).c_str());
					g_verts[index++] = ::atof(((*itr)[2]).c_str());
				}
				g_numIndices = faces.size()*3; // change 3 to dynamically based on face size
				g_indices = new Alembic::Abc::int32_t[g_numIndices];
				//fill...
				index = 0;
				for (std::list<std::vector<std::string> >::const_iterator itr = faces.begin(); itr !=faces.end(); ++itr) {
					g_indices[index++] = ::atoi(((*itr)[0]).c_str());
					g_indices[index++] = ::atoi(((*itr)[1]).c_str());
					g_indices[index++] = ::atoi(((*itr)[2]).c_str());;
				}
				g_numCounts = faces.size();
				g_counts = new Alembic::Abc::int32_t[g_numCounts];
				//fill...
				index = 0;
				for(unsigned int i = 0; i < g_numCounts; ++i) {
					g_counts[index++] = 3;
				}

			return true;


	}
	 bool ABCOutputMesh::writeMesh(const char *meshName) {

/*
				// Open an Alembic file for writing
		Alembic::AbcGeom::OArchive archive(
			Alembic::AbcCoreHDF5::WriteArchive(),
			meshName);
		Alembic::AbcGeom::OPolyMesh meshyObj(Alembic::AbcGeom::OObject(archive, Alembic::Abc::kTop), "mesh");
		Alembic::AbcGeom::OPolyMeshSchema &mesh = meshyObj.getSchema();

		Alembic::AbcGeom::OV2fGeomParam::Sample uvsamp(V2fArraySample((const V2f *)g_uvs, g_numUVs),kFacevaryingScope );
		Alembic::AbcGeom::ON3fGeomParam::Sample nsamp(N3fArraySample((const N3f *)g_normals, g_numNormals),kFacevaryingScope);

		OPolyMeshSchema::Sample mesh_samp(
			P3fArraySample((const V3f *)g_verts, g_numVerts),
			Int32ArraySample(g_indices, g_numIndices),
			Int32ArraySample(g_counts, g_numCounts));
//			,uvsamp, nsamp);

//		Box3d cbox;
//		cbox.extendBy(V3d(1.0, -1.0, 0.0));
//		cbox.extendBy(V3d(-1.0, 1.0, 3.0));

		mesh.set(mesh_samp);
		mesh.set(mesh_samp);

//		std::cout << "Writing: " << archive.getName() << std::endl;
*/
		return true;
}



}
}
