#include <boost/python.hpp>
#include "OBJInputPlugin.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include <vector>
using namespace boost::python;
using namespace MeshPotato::MPMesh;
struct InputMeshWrap: MeshPotato::MPMesh::InputMesh, wrapper<MeshPotato::MPMesh::InputMesh> {
        bool loadMesh(const char *, const MeshPotato::MPUtils::AttributeTable&) {
		return this->get_override("loadMesh")();
	}
        const unsigned int getNumberVertices() const {
                return this->get_override("getNumberVertices")();
        }

};

BOOST_PYTHON_MODULE(pyOBJInputPlugin) {
        class_<InputMeshWrap, boost::noncopyable>("InputMesh", no_init)
                .def("loadMesh", pure_virtual(&MeshPotato::MPMesh::InputMesh::loadMesh))
                .def("getNumberVertices", pure_virtual(&MeshPotato::MPMesh::InputMesh::getNumberVertices))
                ;
	class_<MeshPotato::MPMesh::OBJInputMesh, bases<InputMeshWrap> >("OBJInputMesh")
		.def("loadMesh", &OBJInputMesh::loadMesh)
		.def("getNumberVertices", &OBJInputMesh::getNumberVertices)
		.def("getVertices", &OBJInputMesh::getVertices)
		.def("getNormals", &OBJInputMesh::getNormals)
		.def("getFaces", &OBJInputMesh::getFaces)
		; 
}

