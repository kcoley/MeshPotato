#include <MeshPotato/MPMesh/MeshObject.h>
#include <openvdb/openvdb.h>
#include <pyopenvdb.h>
#include <boost/python.hpp>
#include <MeshPotato/MPMesh/MPMesh.h>
#include <MeshPotato/MPPlugins/pluginapi.h>
#include "pymeshpotato.h"
#include <MeshPotato/meshpotato.h>
#include <MeshPotato/MPPlugins/coreapi.h>
using namespace boost::python;
/*
object processGrid(object inObj) {
	object outObj;
	try {
		if (openvdb::GridBase::Ptr grid = pyopenvdb::getGridFromPyObject(inObj)) {
		grid = grid->deepCopyGrid();
		//process...
		MeshPotato::MPMesh::MeshObject mesh; //dummy example
		outObj = pyopenvdb::getPyObjectFromGrid(grid);
		}
	}
	catch (openvdb::TypeError& e) {
		PyErr_Format(PyExc_TypeError, e.what());
		throw_error_already_set();
	}
	return outObj;
}
*/
struct InputMeshWrap: MeshPotato::MPMesh::InputMesh, wrapper<MeshPotato::MPMesh::InputMesh> {
	bool loadMesh(const char *) {
		return this->get_override("loadMesh")();
	}
	const unsigned int getNumberVertices() const {
		return this->get_override("getNumberVertices")();
	}
};

object mpmesh() {
	object outObj = (
      class_<MeshPotato::MPMesh::MeshObject>("MeshObject", init<>())
              .def("loadMesh", &MeshPotato::MPMesh::MeshObject::loadMesh, "This loads a mesh object")
              .def("writeMesh", &MeshPotato::MPMesh::MeshObject::writeMesh)
	);
	return outObj;
}
object mpmesh2() {
	object outObj = (
      class_<MeshPotato::MPMesh::MPMesh>("MPMesh", no_init)
              .def("SetName", &MeshPotato::MPMesh::MPMesh::SetName)
              .def("GetName", &MeshPotato::MPMesh::MPMesh::GetName)
	);
	return outObj;
}

BOOST_PYTHON_MODULE(mpmesh) {
	openvdb::initialize();
	class_<std::list<int> >("mp_int_list")
		.def("getListFromPython", &getListFromPython<int>)
	;
	class_<std::list<std::vector<std::string> > >("list_vec_str")
                .def("getPythonList", &getPythonList)
        ;
	class_<MeshPotato::MeshSpec>("MeshSpec", init<>())
		.def(init<float, float, float>())
		.def_readwrite("voxelSize", &MeshPotato::MeshSpec::voxelSize)
		.def_readwrite("exBandWidth", &MeshPotato::MeshSpec::exBandWidth)
		.def_readwrite("inBandWidth", &MeshPotato::MeshSpec::inBandWidth)
	;
//	boost::python::def("processGrid", &processGrid, "grid");
	boost::python::def("mpmesh", &mpmesh2);
	class_<MeshPotato::MPMesh::MPMesh>("MPMesh", init<>())
	      .def(init<std::string>())
              .def("SetName", &MeshPotato::MPMesh::MPMesh::SetName)
              .def("GetName", &MeshPotato::MPMesh::MPMesh::GetName)
        ;
	class_<MeshPotato::MPMesh::MeshObject>("MeshObject", init<>())
              .def("loadMesh", &MeshPotato::MPMesh::MeshObject::loadMesh)
              .def("writeMesh", &MeshPotato::MPMesh::MeshObject::writeMesh)
        ;
	class_<InputMeshWrap, boost::noncopyable>("InputMesh", no_init)
		.def("loadMesh", pure_virtual(&MeshPotato::MPMesh::InputMesh::loadMesh))
		.def("getNumberVertices", pure_virtual(&MeshPotato::MPMesh::InputMesh::getNumberVertices))
		;

	def("getPythonList", &getPythonList);
}
