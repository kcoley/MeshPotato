#include <boost/python.hpp>
#include <MPUtils/Color.h>
#include <MPUtils/Camera.h>
#include <MPUtils/Image.h>
#include <MPUtils/DeepImage.h>
#include <MPUtils/Vector.h>
#include <openvdb/openvdb.h>
using namespace boost::python;
using namespace MeshPotato::MPUtils;
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

BOOST_PYTHON_MODULE(mputils) {
	class_<openvdb::Coord>("openvdb_coord")
		.def("getVDBCoord", &getVDBCoord)
		;
	class_<MPVec3>("mpvec3")
		.def("getMPVec3", &getMPVec3)
		;
	class_<openvdb::CoordBBox  >("coord_bbox", no_init)
	        .def("getVDBCoordBBox", &getVDBCoord)
		;
	class_<MeshPotato::MPUtils::Color>("Color")
		.def(init<float, float, float, float>())
		.def("set", &MeshPotato::MPUtils::Color::set)
		;
	class_<MeshPotato::MPUtils::Image>("Image", no_init)
		.def("__init__", make_constructor(&MeshPotato::MPUtils::Image::Ptr))
		.def("reset", &MeshPotato::MPUtils::Image::reset)
		;
	class_<MeshPotato::MPUtils::DeepImage>("DeepImage", no_init)
		.def("__init__", make_constructor(&MeshPotato::MPUtils::DeepImage::Ptr))
		.def("reset", &MeshPotato::MPUtils::DeepImage::reset)
		;
	class_<MeshPotato::MPUtils::Camera>("Camera", no_init)
		.def("__init__", make_constructor(&MeshPotato::MPUtils::Camera::Ptr))
		.def("setEyeViewUp", &MeshPotato::MPUtils::Camera::setEyeViewUp)
		;
	class_<boost::shared_ptr<MeshPotato::MPUtils::Camera> >("camera")
		;
	class_<boost::shared_ptr<MeshPotato::MPUtils::Image> >("image")
		;
	class_<boost::shared_ptr<MeshPotato::MPUtils::DeepImage> >("deepimage")
		;
	def("getVDBCoord", &getVDBCoord);
	def("getVDBCoordBBox", &getVDBCoordBBox);
	def("getMPVec3", &getMPVec3);
}
