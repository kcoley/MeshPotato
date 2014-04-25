#include <boost/python.hpp>
#include <MPUtils/Color.h>
#include <MPUtils/Camera.h>
#include <MPUtils/Image.h>
#include <MPUtils/DeepImage.h>
#include <MPUtils/Vector.h>
#include <openvdb/openvdb.h>
#include <MPUtils/AttributeTable.h>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
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
object getCoord(boost::python::list& ls) {
	MeshPotato::MPUtils::Coord coord;
	if (len(ls) == 3) {
		coord.setX(boost::python::extract<float>(ls[0]));
		coord.setY(boost::python::extract<float>(ls[1]));
		coord.setZ(boost::python::extract<float>(ls[2]));
	}
	else
		std::cout << "List is not of three elements" << std::endl;
	return object(coord);
}
openvdb::CoordBBox getVDBCoordBBoxFunc(openvdb::Coord &min, openvdb::Coord &max) {
	return openvdb::CoordBBox(min, max);
}
object getVDBCoordBBox(object min_obj, object max_obj) {
	openvdb::Coord min = boost::python::extract<openvdb::Coord>(min_obj);
	openvdb::Coord max = boost::python::extract<openvdb::Coord>(max_obj);

	return object(getVDBCoordBBoxFunc(min, max));
}

object getBBox(object min_obj, object max_obj) {
	MeshPotato::MPUtils::MPVec3 min = boost::python::extract<MeshPotato::MPUtils::MPVec3>(min_obj);
	MeshPotato::MPUtils::MPVec3 max = boost::python::extract<MeshPotato::MPUtils::MPVec3>(max_obj);

	return object(MeshPotato::MPUtils::BBox(min,max));
}
BOOST_PYTHON_MODULE(mputils) {
	class_<openvdb::Coord>("openvdb_coord")
		.def("getVDBCoord", &getVDBCoord)
		;
	class_<MeshPotato::MPUtils::Coord>("Coord")
		.def("getCoord", &getCoord)
		;
	class_<MPVec3>("mpvec3")
		.def("getMPVec3", &getMPVec3)
		;
	class_<MeshPotato::MPUtils::BBox>("bbox")
		.def("getBBox", &getBBox)
		;
	class_<openvdb::CoordBBox  >("coord_bbox", no_init)
	        .def("getVDBCoordBBox", &getVDBCoord)
		;
	class_<MeshPotato::MPUtils::Color>("Color")
		.def(init<float, float, float, float>())
		.def("set", &MeshPotato::MPUtils::Color::set)
		;
	class_<MeshPotato::MPUtils::AttributeTable>("AttributeTable")
		.def(init<>())
		.def("addIntAttr", &MeshPotato::MPUtils::AttributeTable::addIntAttr)
		.def("addDoubleAttr", &MeshPotato::MPUtils::AttributeTable::addDoubleAttr)
		.def("addVectorAttr", &MeshPotato::MPUtils::AttributeTable::addVectorAttr)
		.def("addStringAttr", &MeshPotato::MPUtils::AttributeTable::addStringAttr)
		.def("findIntAttr", &MeshPotato::MPUtils::AttributeTable::findIntAttr)
		.def("findDoubleAttr", &MeshPotato::MPUtils::AttributeTable::findDoubleAttr)
		.def("findVectorAttr", &MeshPotato::MPUtils::AttributeTable::findVectorAttr)
		.def("findStringAttr", &MeshPotato::MPUtils::AttributeTable::findStringAttr)
		.def("stringAttrNames", &MeshPotato::MPUtils::AttributeTable::stringAttrNames)
		.def("intAttrNames", &MeshPotato::MPUtils::AttributeTable::intAttrNames)
		.def("doubleAttrNames", &MeshPotato::MPUtils::AttributeTable::doubleAttrNames)
		.def("vectorAttrNames", &MeshPotato::MPUtils::AttributeTable::vectorAttrNames)
		.def("mergeTable", &MeshPotato::MPUtils::AttributeTable::mergeTable)
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
	def("getCoord", &getCoord);
	def("getVDBCoordBBox", &getVDBCoordBBox);
	def("getMPVec3", &getMPVec3);
	def("getBBox", &getBBox);
}
