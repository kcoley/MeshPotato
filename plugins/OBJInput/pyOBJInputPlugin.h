#include <boost/python.hpp>
#include "OBJInputPlugin.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include <vector>
using namespace boost::python;
using namespace MeshPotato::MPPlugins;
struct InputMeshWrap: MeshPotato::MPPlugins::InputMeshAPI, wrapper<MeshPotato::MPPlugins::InputMeshAPI> {
        bool loadMesh(const char *) {}
        bool writeMesh(const char *) {}
        const unsigned int getNumberVertices() const {
                return this->get_override("getNumberVertices")();
        }

};

BOOST_PYTHON_MODULE(pyOBJInputPlugin) {
//        openvdb::initialize();
        class_<InputMeshWrap, boost::noncopyable>("InputMesh", no_init)
                .def("loadMesh", pure_virtual(&MeshPotato::MPPlugins::InputMeshAPI::loadMesh))
                .def("getNumberVertices", pure_virtual(&MeshPotato::MPPlugins::InputMeshAPI::getNumberVertices))
                ;
	class_<MeshPotato::MPPlugins::OBJInputMesh, bases<InputMeshWrap> >("OBJInputMesh")
		.def("loadMesh", &OBJInputMesh::loadMesh)
		.def("getNumberVertices", &OBJInputMesh::getNumberVertices)
		.def("getVertices", &OBJInputMesh::getVertices)
		.def("getNormals", &OBJInputMesh::getNormals)
		.def("getFaces", &OBJInputMesh::getFaces)
		; 
}

