#include <boost/python.hpp>
#include "OBJOutputPlugin.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include <vector>
using namespace boost::python;
using namespace MeshPotato::MPMesh;
struct OutputMeshWrap: MeshPotato::MPMesh::OutputMesh, wrapper<MeshPotato::MPMesh::OutputMesh> {
        bool loadMesh(const char *, const MeshPotato::MPUtils::AttributeTable &table) {
	return this->get_override("loadMesh")();
	}
        bool writeMesh(const char *) {
	return this->get_override("writeMesh")();
	}
        const unsigned int getNumberVertices() const {
                return this->get_override("getNumberVertices")();
        }

};

BOOST_PYTHON_MODULE(pyOBJOutputPlugin) {
        class_<OutputMeshWrap, boost::noncopyable>("OutputMesh", no_init)
                .def("loadMesh", pure_virtual(&MeshPotato::MPMesh::OutputMesh::loadMesh))
                .def("getNumberVertices", pure_virtual(&MeshPotato::MPMesh::OutputMesh::getNumberVertices))
                ;
	class_<OBJOutputMesh, bases<OutputMeshWrap> >("OBJOutputMesh")
		.def("loadMesh", &OBJOutputMesh::loadMesh)
		.def("writeMesh", &OBJOutputMesh::writeMesh)
		.def("getNumberVertices", &OBJOutputMesh::getNumberVertices)
		;
}

