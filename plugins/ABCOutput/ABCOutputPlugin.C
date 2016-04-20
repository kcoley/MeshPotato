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

  bool ABCOutputMesh::loadMesh(
				const std::list<std::vector<std::string> > &vertices,
				const std::list<std::vector<std::string> > &normals,
				const std::list<std::vector<std::string> > &faces,
				const MeshPotato::MPUtils::AttributeTable &table) {


				int index = 0;
				this->vertices = vertices;
				this->normals = normals;
				this->faces = faces;

				// Convert to alembic friendly versions
				g_numVerts = vertices.size();
				g_numIndices = faces.size()*3; // change 3 to dynamically based on face size
				g_numNormals = g_numIndices;
				g_numCounts = faces.size();
				g_verts =   new Alembic::Abc::float32_t[g_numVerts*3];
				g_normals = new Alembic::Abc::float32_t[g_numNormals*3];

				std::cout << "number of verts = " << g_numVerts << "\n num normals " << g_numNormals << "\n num faces " << g_numCounts << std::endl;
				//fill...

				//First, we get all the vertices

				for (std::list<std::vector<std::string> >::const_iterator itr = vertices.begin(); itr != vertices.end(); ++itr) {
					g_verts[index++] = ::atof(((*itr)[0]).c_str());
					g_verts[index++] = ::atof(((*itr)[1]).c_str());
					g_verts[index++] = ::atof(((*itr)[2]).c_str());
				}

				//Next we get all the indices
				g_indices = new Alembic::Abc::int32_t[g_numIndices];
				//fill...
				index = 0;
				for (std::list<std::vector<std::string> >::const_iterator itr = faces.begin(); itr !=faces.end(); ++itr) {
					g_indices[index++] = ::atoi(((*itr)[0]).c_str()) -1;
					g_indices[index++] = ::atoi(((*itr)[1]).c_str()) -1;
					g_indices[index++] = ::atoi(((*itr)[2]).c_str()) -1;
				}


				// Next, we get all the normals per index
				index = 0;
				std::cout << "number of normals = " << normals.size() << std::endl;
				std::vector<std::vector<std::string> > new_normals;
				new_normals.reserve(normals.size());
				std::copy(std::begin(normals), std::end(normals), std::back_inserter(new_normals));
				index = 0;

				for (std::list<std::vector<std::string> >::const_iterator itr = normals.begin(); itr !=normals.end(); ++itr) {
					g_normals[index++] = ::atof(((*itr)[0]).c_str());
					g_normals[index++] = ::atof(((*itr)[1]).c_str());
					g_normals[index++] = ::atof(((*itr)[2]).c_str());
				}
/*
				for (int i = 0; i < g_numIndices; ++i) {
					std::vector<std::string > normal = new_normals[g_indices[i]];
					std::cout << (::atof((normal[0]).c_str())) << " " << (::atof((normal[1]).c_str())) << " " << (::atof((normal[2]).c_str())) << std::endl;
					g_normals[index++] = (::atof((normal[0]).c_str()));
					g_normals[index++] = (::atof((normal[1]).c_str()));
					g_normals[index++] = (::atof((normal[2]).c_str()));
				}

*/


				g_counts = new Alembic::Abc::int32_t[g_numCounts];
				//fill...
				index = 0;
				for (std::list<std::vector<std::string> >::const_iterator iter = faces.begin(); iter != faces.end(); ++iter) {
					g_counts[index++] =(*iter).size();
				}
			/*	for(unsigned int i = 0; i < g_numCounts; ++i) {
					g_counts[index++] = 3;
				}
				*/

			return true;


	}
	 bool ABCOutputMesh::writeMesh(const char *meshName) {


				// Open an Alembic file for writing
		Alembic::AbcGeom::OArchive archive(
			Alembic::AbcCoreHDF5::WriteArchive(),
			meshName
		);

		Alembic::AbcGeom::OPolyMesh meshyObj(
			Alembic::AbcGeom::OObject(archive, Alembic::Abc::kTop),
			"mesh"
		);

		Alembic::AbcGeom::OPolyMeshSchema &mesh = meshyObj.getSchema();

	//	Alembic::AbcGeom::OV2fGeomParam::Sample uvsamp(V2fArraySample((const V2f *)g_uvs, g_numUVs),kFacevaryingScope );
		Alembic::AbcGeom::ON3fGeomParam::Sample nsamp(
			N3fArraySample((const N3f *)g_normals, g_numNormals),
			kFacevaryingScope
		);

std::cout << "test..." << std::endl;
Alembic::AbcGeom::OPolyMeshSchema::Sample mesh_samp(
	P3fArraySample((const V3f *)g_verts, g_numVerts),
	Int32ArraySample(g_indices, g_numIndices),
	Int32ArraySample(g_counts, g_numCounts),
	OV2fGeomParam::Sample(),
	nsamp
);
/*
		Alembic::AbcGeom::OPolyMeshSchema::Sample mesh_samp(
			P3fArraySample((const V3f *)g_verts, g_numVerts),
			Int32ArraySample(g_indices, g_numIndices),
			Int32ArraySample(g_counts, g_numCounts));
			*/
std::cout << "test..." << std::endl;

//			,uvsamp, nsamp);



		mesh.set(mesh_samp);
		//mesh.set(mesh_samp);

		std::cout << "Writing: " << archive.getName() << std::endl;

		return true;
}



}
}
