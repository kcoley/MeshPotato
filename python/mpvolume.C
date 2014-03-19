#include <boost/python.hpp>
#include <MPVolume/MPVolume.h>
#include <MPVolume/VDBVolumeGrid.h>
#include <MPVolume/FieldAlgebra.h>
#include <MPVolume/VolumeUtils.h>
#include <MPVolume/ImplicitShape.h>
#include <MPVolume/RayMarcher.h>
#include <MPVolume/FrustumGrid.h>
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
MPVec3 getMPVec3(boost::python::list& ls) {
	MPVec3 vec;
	if (len(ls) == 3) {
		vec.x() = (boost::python::extract<float>(ls[0]));
		vec.y() = (boost::python::extract<float>(ls[1]));
		vec.z() = (boost::python::extract<float>(ls[2]));
	}
	return vec;
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
	class_<MPVec3>("mpvec3")
		.def("getMPVec3", &getMPVec3)
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
	class_<AddVolumeFloat,  bases<MPVolumeWrap> >("AddVolumeFloat", no_init)
		.def(init<boost::shared_ptr<Volume<float> >, boost::shared_ptr<Volume<float> > >())
		.def("ptr", &AddVolumeFloat::Ptr)
		.staticmethod("ptr")
		.def("eval", &AddVolumeFloat::eval)
		.def("grad", &AddVolumeFloat::grad)
	;
	class_<ImplicitSphere, bases<MPVolumeWrap> >("ImplicitSphere", no_init)
		.def(init<float, MeshPotato::MPUtils::MPVec3>() )
		.def("ptr", &ImplicitSphere::Ptr)
		.staticmethod("ptr")
		.def("eval", &ImplicitSphere::eval)
		.def("grad", &ImplicitSphere::grad)
	;
	class_<Clamp<float>, bases<MPVolumeWrap> >("Clamp", no_init)
		.def(init<boost::shared_ptr<Volume<float> >, float, float>())
		.def("ptr", &MeshPotato::MPVolume::Clamp<float>::Ptr)
		.staticmethod("ptr")
		.def("eval", &MeshPotato::MPVolume::Clamp<float>::eval)
		.def("grad", &MeshPotato::MPVolume::Clamp<float>::grad)
	;
//	class_<VDBRayMarcher>("VDBRayMarcher", no_init)
//		.def(init<	
	def("makeVDBGrid", &MeshPotato::MPVolume::makeVDBGrid);
	def("getMPVolumeFromPyObject", &getMPVolumeFromPyObject);
	def("getVDBCoord", &getVDBCoord);
	def("getVDBCoordBBox", &getVDBCoordBBox);
	def("getMPVec3", &getMPVec3);
}
