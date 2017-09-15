#include <boost/python.hpp>
#include "VDBOutputPlugin.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include <vector>
#include <MeshPotato/MPUtils/AttributeTable.h>
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

BOOST_PYTHON_MODULE(pyVDBOutputPlugin) {
        class_<OutputMeshWrap, boost::noncopyable>("OutputMesh", no_init)
                .def("loadMesh", pure_virtual(&MeshPotato::MPMesh::OutputMesh::loadMesh))
                .def("getNumberVertices", pure_virtual(&MeshPotato::MPMesh::OutputMesh::getNumberVertices))
                .def("writeMesh", pure_virtual(&MeshPotato::MPMesh::OutputMesh::writeMesh))
                ;
	class_<VDBOutputMesh, bases<OutputMeshWrap> >("VDBOutputMesh")
		.def("loadMesh", &VDBOutputMesh::loadMesh)
		.def("writeMesh", &VDBOutputMesh::writeMesh)
		.def("getNumberVertices", &VDBOutputMesh::getNumberVertices)
		;
}
