#include <openvdb/openvdb.h>
#include <pyopenvdb.h>
#include <boost/python.hpp>
#include <MPMesh/MeshObject.h>
#include <MPMesh/MPMesh.h>
#include <MPPlugins/InputMesh.h>
using namespace boost::python;
object processGrid(object inObj) {
	object outObj;
	try {
		if (openvdb::GridBase::Ptr grid = pyopenvdb::getGridFromPyObject(inObj)) {
		grid = grid->deepCopyGrid();
		//process...
		MeshPotato::MeshObject mesh; //dummy example
		outObj = pyopenvdb::getPyObjectFromGrid(grid);
		}
	}
	catch (openvdb::TypeError& e) {
		PyErr_Format(PyExc_TypeError, e.what());
		throw_error_already_set();
	}
	return outObj;
}

struct InputMeshWrap: MyEngine::InputMesh, wrapper<MyEngine::InputMesh> {
	void loadMesh(const char *) {}
	const size_t getNumberVertices() const {
		return this->get_override("getNumberVertices")();
	}

};
object mpmesh() { 
	object outObj = (	
      class_<MeshPotato::MeshObject>("MeshObject", no_init)
              .def("loadMesh", &MeshPotato::MeshObject::loadMesh)
              .def("writeMesh", &MeshPotato::MeshObject::writeMesh)
	);
	return outObj;
}
object mpmesh2() { 
	object outObj = (	
      class_<MeshPotato::MPMesh::MPMesh>("MPMesh", no_init)
              .def("SetName", &MeshPotato::MPMesh::MPMesh::SetName)
              .def("GetName", &MeshPotato::MPMesh::MPMesh::GetName)
//              .def("Write", &MeshPotato::MPMesh::MPMesh::Write)
	);
	return outObj;
}

BOOST_PYTHON_MODULE(mpmesh) {
	openvdb::initialize();
	boost::python::def("processGrid", &processGrid, "grid");
	boost::python::def("mpmesh", &mpmesh2);
	class_<MeshPotato::MPMesh::MPMesh>("MPMesh", init<>())
	      .def(init<std::string>())
              .def("SetName", &MeshPotato::MPMesh::MPMesh::SetName)
              .def("GetName", &MeshPotato::MPMesh::MPMesh::GetName)
//              .def("Write", &MeshPotato::MPMesh::MPMesh::Write)
        ;
	class_<InputMeshWrap, boost::noncopyable>("InputMesh")
		.def("loadMesh", pure_virtual(&MyEngine::InputMesh::InputMeshDriver::loadMesh))
		.def("getNumberVertices", pure_virtual(&MyEngine::InputMesh::InputMeshDriver::getNumberVertices))
		;
//	class_<MeshPotato::MPMesh::MPMesh>("MPMesh", no_init)
//		.def("SetName", &MeshPotato::MPMesh::MPMesh::SetName)
//	;
}

