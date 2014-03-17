#include <boost/python.hpp>
#include "VDBOutputPlugin.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include <vector>
using namespace boost::python;
using namespace MeshPotato::MPPlugins;
struct OutputMeshWrap: MeshPotato::MPPlugins::OutputMeshAPI, wrapper<MeshPotato::MPPlugins::OutputMeshAPI> {
        bool loadMesh(const char *) {}
        bool writeMesh(const char *) {}
        const unsigned int getNumberVertices() const {
                return this->get_override("getNumberVertices")();
        }

};

BOOST_PYTHON_MODULE(pyVDBOutputPlugin) {
        class_<OutputMeshWrap, boost::noncopyable>("OutputMesh", no_init)
                .def("loadMesh", pure_virtual(&MeshPotato::MPPlugins::OutputMeshAPI::loadMesh))
                .def("getNumberVertices", pure_virtual(&MeshPotato::MPPlugins::OutputMeshAPI::getNumberVertices))
                ;
	class_<VDBOutputMesh, bases<OutputMeshWrap> >("VDBOutputMesh")
		.def("loadMesh", &VDBOutputMesh::loadMesh)
		.def("writeMesh", &VDBOutputMesh::writeMesh)
		.def("getNumberVertices", &VDBOutputMesh::getNumberVertices)
		;
}

