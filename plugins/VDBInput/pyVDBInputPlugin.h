#include <boost/python.hpp>
#include "VDBInputPlugin.h"
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

BOOST_PYTHON_MODULE(pyVDBInputPlugin) {
//        openvdb::initialize();
        class_<InputMeshWrap, boost::noncopyable>("InputMesh", no_init)
                .def("loadMesh", pure_virtual(&MeshPotato::MPPlugins::InputMeshAPI::loadMesh))
                .def("getNumberVertices", pure_virtual(&MeshPotato::MPPlugins::InputMeshAPI::getNumberVertices))
                ;


	class_<VDBInputMesh, bases<InputMeshWrap> >("VDBInputMesh")
		.def("loadMesh", &VDBInputMesh::loadMesh)
		.def("getNumberVertices", &VDBInputMesh::getNumberVertices)
		.def("getVertices", &VDBInputMesh::getVertices)
		.def("getNormals", &VDBInputMesh::getNormals)
		.def("getFaces", &VDBInputMesh::getFaces)
		; 
}

