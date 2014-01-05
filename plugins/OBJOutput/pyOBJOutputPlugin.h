#include <boost/python.hpp>
#include "OBJOutputPlugin.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <list>
#include <vector>
using namespace boost::python;
using namespace MyEngine;
struct OutputMeshWrap: MyEngine::OutputMesh, wrapper<MyEngine::OutputMesh> {
        void loadMesh(const char *) {}
        void writeMesh(const char *) {}
        const size_t getNumberVertices() const {
                return this->get_override("getNumberVertices")();
        }

};

BOOST_PYTHON_MODULE(pyOBJOutputPlugin) {
//        openvdb::initialize();
        class_<OutputMeshWrap, boost::noncopyable>("OutputMesh")
                .def("loadMesh", pure_virtual(&MyEngine::OutputMesh::OutputMeshDriver::loadMesh))
                .def("getNumberVertices", pure_virtual(&MyEngine::OutputMesh::OutputMeshDriver::getNumberVertices))
                ;
	class_<OBJOutputMeshDriver, bases<OutputMeshWrap> >("OBJOutputMesh")
		.def("loadMesh", &OBJOutputMeshDriver::loadMesh)
		.def("writeMesh", &OBJOutputMeshDriver::writeMesh)
		.def("getNumberVertices", &OBJOutputMeshDriver::getNumberVertices)
		;
}

