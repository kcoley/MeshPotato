#include <boost/python.hpp>
#include "VDBInputPlugin.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include <vector>
#include <MPUtils/AttributeTable.h>
using namespace boost::python;
using namespace MeshPotato::MPMesh;

struct InputMeshWrap:MeshPotato::MPMesh::InputMesh, wrapper<MeshPotato::MPMesh::InputMesh> {
        bool loadMesh(const char *, const MeshPotato::MPUtils::AttributeTable) {
		return this->get_override("loadMesh")();
	}
        const unsigned int getNumberVertices() const {
                return this->get_override("getNumberVertices")();
        }

};

BOOST_PYTHON_MODULE(pyVDBInputPlugin) {
//        openvdb::initialize();
        class_<InputMeshWrap, boost::noncopyable>("InputMesh", no_init)
                .def("loadMesh", pure_virtual(&MeshPotato::MPMesh::InputMesh::loadMesh))
                .def("getNumberVertices", pure_virtual(&MeshPotato::MPMesh::InputMesh::getNumberVertices))
                ;


	class_<VDBInputMesh, bases<InputMeshWrap> >("VDBInputMesh")
		.def("loadMesh", &VDBInputMesh::loadMesh)
		.def("getNumberVertices", &VDBInputMesh::getNumberVertices)
		.def("getVertices", &VDBInputMesh::getVertices)
		.def("getNormals", &VDBInputMesh::getNormals)
		.def("getFaces", &VDBInputMesh::getFaces)
		; 
}

