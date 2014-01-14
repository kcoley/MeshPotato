#include <boost/python.hpp>
#include <MPVolume/MPVolume.h>
#include <MPVolume/VDBVolumeGrid.h>
#include <MPVolume/FieldAlgebra.h>
#include <MPVolume/VDBVolumeUtils.h>
#include <MPVolume/ImplicitShape.h>

using namespace boost::python;
using namespace MeshPotato::MPVolume;
using namespace MeshPotato::MPUtils;
struct MPVolumeWrap : Volume<float>, wrapper<Volume<float> > {
	const float eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
	const MPVec3 grad(const MPVec3& P) const {
		return this->get_override("grad")();
	}
};
boost::python::object getMPVolumeFromPyObject(boost::shared_ptr<Volume<float> >vdbgrid) {
	return object(&vdbgrid);
}

openvdb::Coord getVDBCoord(boost::python::list& ls) {
	openvdb::Coord coord;
	if (len(ls) == 3) {
		coord.setX(boost::python::extract<float>(ls[0]));
		coord.setY(boost::python::extract<float>(ls[1]));
		coord.setZ(boost::python::extract<float>(ls[2]));
	}
	else
		std::cout << "List is not of three elements" << std::endl;
	return coord;
}
openvdb::CoordBBox getVDBCoordBBox(openvdb::Coord &min, openvdb::Coord &max) {

	return openvdb::CoordBBox(min, max);
}
BOOST_PYTHON_MODULE(mpvolume) {
	class_<openvdb::Coord>("openvdb_coord")
		.def("getVDBCoord", &getVDBCoord)
	;
	class_<boost::shared_ptr<Volume<float> > >("mpvolume_float")
	;
	class_<openvdb::CoordBBox  >("coord_bbox", no_init)
		.def("getVDBCoordBBox", &getVDBCoord)
	;
	class_<MPVolumeWrap, boost::noncopyable>("VolumeFloat")
		.def("eval", pure_virtual(&Volume<float>::eval))	
		.def("grad", pure_virtual(&Volume<float>::grad))	
	;
	class_<VDBVolumeGrid, boost::shared_ptr<VDBVolumeGrid>, bases<MPVolumeWrap> >("VDBVolumeGrid", no_init)
		.def(init<openvdb::FloatGrid::Ptr>())
		.def("ptr", &VDBVolumeGrid::Ptr)
		.staticmethod("ptr")
		.def("eval", &VDBVolumeGrid::eval)
		.def("grad", &VDBVolumeGrid::grad)
	;
	class_<AddVolume<float, Volume<float> >,  bases<MPVolumeWrap> >("AddVolume", no_init)
		.def(init<boost::shared_ptr<Volume<float> >, boost::shared_ptr<Volume<float> > >())
		.def("ptr", &AddVolume<float, Volume<float> >::Ptr)
		.staticmethod("ptr")
		.def("eval", &AddVolume<float, Volume<float> >::eval)
		.def("grad", &AddVolume<float, Volume<float> >::grad)
	;
	class_<ImplicitSphere, bases<MPVolumeWrap> >("ImplicitSphere", no_init)
		.def(init<float, MeshPotato::MPUtils::MPVec3>() )
		.def("ptr", &ImplicitSphere::Ptr)
		.staticmethod("ptr")
		.def("eval", &ImplicitSphere::eval)
	;
	class_<Clamp<float>, bases<MPVolumeWrap> >("Clamp", no_init)
		.def(init<boost::shared_ptr<Volume<float> >, float, float>())
		.def("ptr", &MeshPotato::MPVolume::Clamp<float>::Ptr)
		.staticmethod("ptr")
		.def("eval", &MeshPotato::MPVolume::Clamp<float>::eval)
		.def("grad", &MeshPotato::MPVolume::Clamp<float>::grad)
	;	
	def("makeVDBGrid", &MeshPotato::MPVolume::makeVDBGrid);
	def("getMPVolumeFromPyObject", &getMPVolumeFromPyObject);
	def("getVDBCoord", &getVDBCoord);
	def("getVDBCoordBBox", &getVDBCoordBBox);
}
