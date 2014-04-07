#include <boost/python.hpp>
#include <MPVolume/MPVolume.h>
#include <MPVolume/VDBVolumeGrid.h>
#include <MPVolume/FieldAlgebra.h>
#include <MPVolume/VolumeUtils.h>
#include <MPVolume/ImplicitShape.h>
#include <MPVolume/RayMarcher.h>
#include <MPVolume/FrustumGrid.h>
#include <MPVolume/Light.h>
#include <MPVolume/VolumeUtils.h>
#include <MPUtils/Camera.h>
#include <string>
#include <openvdb/openvdb.h>
#include <openvdb/tools/LevelSetUtil.h>
using namespace boost::python;
using namespace MeshPotato::MPVolume;
using namespace MeshPotato::MPUtils;

object ConvertToFogVolume(object &grid) {
	openvdb::FloatGrid grid_obj = extract<openvdb::FloatGrid>(grid);
	openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(grid_obj);

	return object(grid_obj);

}

MeshPotato::MPVolume::VolumeColorPtr CreateFrustumLight(MPVec3 light_pos, Color light_color, MPVec3 light_resolution, openvdb::FloatGrid grid, float dsmK=1.0) {
	openvdb::FloatGrid::Ptr gridPtr = grid.copy();
	boost::shared_ptr<MeshPotato::MPUtils::Camera> frustumCam = MeshPotato::MPVolume::buildFrustumCamera(light_pos, gridPtr);
	openvdb::BBoxd bbox(MeshPotato::MPUtils::MPVec3(0,0,0), light_resolution);
	boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid > frustum = MeshPotato::MPVolume::FrustumGrid::Ptr(frustumCam, bbox);
	MeshPotato::MPVolume::VolumeFloatPtr dsmgrid = MeshPotato::MPVolume::VDBVolumeGrid::Ptr(gridPtr);
	frustum->dsm(dsmgrid, dsmK);
	VolumeColorPtr light = MeshPotato::MPVolume::FrustumLight::Ptr(frustum, light_color);
	return light;

};
object CreateVDBFrustumLight(object& light_pos, object& light_color, object& light_resolution, object& grid) {
	extract<openvdb::FloatGrid> grid_obj(grid);
	extract<MPVec3> light_pos_obj(light_pos);
	extract<Color> light_color_obj(light_color);
	extract<MPVec3> light_resolution_obj(light_resolution);
	return object(CreateFrustumLight(light_pos_obj, light_color_obj, light_resolution_obj, grid_obj));

};
void RenderVDBFunc(openvdb::FloatGrid grid, VolumeColorPtr light, double step, double k, boost::shared_ptr<Image> image, boost::shared_ptr<DeepImage> deepimage, boost::shared_ptr<Camera> camera, std::string name, bool threaded) {
	const bool isLevelSet = (grid.getGridClass() == openvdb::GRID_LEVEL_SET);
	openvdb::FloatGrid::Ptr gridPtr = grid.copy();
	if (isLevelSet) {
		//convert to fog volume
		openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(*gridPtr);
	}
	openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> inter(*gridPtr);
	MeshPotato::MPVolume::VDBRayMarcher marcher = MeshPotato::MPVolume::VDBRayMarcher(gridPtr, light, step, k, image, deepimage, camera, name);
	marcher.render(threaded);
	marcher.writeImage();
}
void RenderVDB(object grid, object light, object step, object k, object image, object deepimage, object camera, object name, object threaded) {

	extract<openvdb::FloatGrid> grid_obj(grid);
	extract<VolumeColorPtr> light_obj(light);
	extract<double> step_obj(step);
	extract<double> k_obj(k);
	extract<boost::shared_ptr<Image> > image_obj(image);
	extract<boost::shared_ptr<DeepImage> > deepimage_obj(deepimage);
	extract<boost::shared_ptr<Camera> > camera_obj(camera);
	extract<std::string> name_obj(name);
	extract<bool> threaded_obj(threaded);
	RenderVDBFunc(grid_obj, light_obj, step_obj, k_obj, image_obj, deepimage_obj, camera_obj, name_obj, threaded);
}
openvdb::FloatGrid::Ptr getVDBPtr(openvdb::FloatGrid grid) {
	return grid.copy();
}
struct MPVolumeFloatWrap : Volume<float>, wrapper<Volume<float> > {
	const float eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
	const MPVec3 grad(const MPVec3& P) const {
		return this->get_override("grad")();
	}
};
struct MPVolumeVectorWrap : Volume<MPVec3>, wrapper<Volume<MPVec3> > {
	const MPVec3 eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
	const MPMat3 grad(const MPVec3& P) const {
		return this->get_override("grad")();
	}
};
struct MPVolumeColorWrap : Volume<Color>, wrapper<Volume<Color> > {
	const Color eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
	const int grad(const MPVec3& P) const {
		return this->get_override("grad")();
	}
};
boost::python::object getMPVolumeFromPyObject(
		boost::shared_ptr<Volume<float> >vdbgrid) {
	return object(&vdbgrid);
}

BOOST_PYTHON_MODULE(mpvolume) {
	;
	class_<boost::shared_ptr<FrustumLight> >("frustum_light")
	;
	class_<boost::shared_ptr<Volume<float> > >("mpvolume_float")
	;
	class_<boost::shared_ptr<Volume<Color> > >("mpvolume_color")
	;
	class_<MPVolumeFloatWrap, boost::noncopyable>("VolumeFloat")
		.def("eval", pure_virtual(&Volume<float>::eval))	
		.def("grad", pure_virtual(&Volume<float>::grad))	
	;
	class_<MPVolumeVectorWrap, boost::noncopyable>("VolumeVector")
		.def("eval", pure_virtual(&Volume<MPVec3>::eval))	
		.def("grad", pure_virtual(&Volume<MPMat3>::grad))	
	;
	class_<MPVolumeColorWrap, boost::noncopyable>("VolumeColor")
		.def("eval", pure_virtual(&Volume<Color>::eval))	
		.def("grad", pure_virtual(&Volume<Color>::grad))	
	;
	class_<VDBVolumeGrid, boost::shared_ptr<VDBVolumeGrid>, 
			bases<MPVolumeFloatWrap> >("VDBVolumeGrid", no_init)
		.def(init<openvdb::FloatGrid::Ptr>())
		.def("ptr", &VDBVolumeGrid::Ptr)
		.staticmethod("ptr")
		.def("eval", &VDBVolumeGrid::eval)
		.def("grad", &VDBVolumeGrid::grad)
	;
	class_<AddVolumeFloat,  bases<MPVolumeFloatWrap> >("AddVolumeFloat", no_init)
		.def("__init__", make_constructor(&AddVolumeFloat::Ptr))
		.def("eval", &AddVolumeFloat::eval)
		.def("grad", &AddVolumeFloat::grad)
	;
	class_<UnionFloat,  bases<MPVolumeFloatWrap> >("UnionFloat", no_init)
		.def("__init__", make_constructor(&UnionFloat::Ptr))
		.def("eval", &UnionFloat::eval)
		.def("grad", &UnionFloat::grad)
	;
	class_<UnionVector,  bases<MPVolumeFloatWrap> >("UnionVector", no_init)
		.def("__init__", make_constructor(&UnionVector::Ptr))
		.def("eval", &UnionVector::eval)
		.def("grad", &UnionVector::grad)
	;
	class_<AddVolumeColor,  bases<MPVolumeColorWrap> >("AddVolumeColor", no_init)
		.def("__init__", make_constructor(&AddVolumeColor::Ptr))
		.def("ptr", &AddVolumeColor::Ptr)
		.staticmethod("ptr")
		.def("eval", &AddVolumeColor::eval)
		.def("grad", &AddVolumeColor::grad)
	;
	class_<ImplicitSphere, bases<MPVolumeFloatWrap> >("ImplicitSphere", no_init)
		.def("__init__", make_constructor(&ImplicitSphere::Ptr))
		.def("ptr", &ImplicitSphere::Ptr)
		.staticmethod("ptr")
		.def("eval", &ImplicitSphere::eval)
		.def("grad", &ImplicitSphere::grad)
	;
	class_<PyroclasticSphere, bases<MPVolumeFloatWrap> >("PyroclasticSphere", no_init)
		.def("__init__", make_constructor(&PyroclasticSphere::Ptr))
		.def("eval", &PyroclasticSphere::eval)
		.def("grad", &PyroclasticSphere::grad)
	;
	class_<Clamp<float>, bases<MPVolumeFloatWrap> >("Clamp", no_init)
		.def("__init__", make_constructor(&Clamp<float>::Ptr))
		.def("eval", &MeshPotato::MPVolume::Clamp<float>::eval)
		.def("grad", &MeshPotato::MPVolume::Clamp<float>::grad)
	;
	class_<VectorNoise,bases<MPVolumeVectorWrap> >("VectorNoise", no_init)
		.def("__init__", make_constructor(&VectorNoise::Ptr))
		.def("eval", &MeshPotato::MPVolume::VectorNoise::eval)
		.def("grad", &MeshPotato::MPVolume::VectorNoise::grad)
	;
	class_<AdvectVolume,bases<MPVolumeFloatWrap> >("AdvectVolume", no_init)
		.def("__init__", make_constructor(&AdvectVolume::Ptr))
		.def("eval", &MeshPotato::MPVolume::AdvectVolume::eval)
		.def("grad", &MeshPotato::MPVolume::AdvectVolume::grad)
	;
	class_<FrustumLight, bases<MPVolumeColorWrap> >("FrustumLight", no_init)
		.def("__init__", make_constructor(&FrustumLight::Ptr))
	;	
	class_<VDBRayMarcher>("VDBRayMarcher", no_init)
		.def(init<openvdb::FloatGrid::Ptr, VolumeColorPtr, double, double, boost::shared_ptr<Image>, boost::shared_ptr<DeepImage> , boost::shared_ptr<Camera> , std::string>())
		.def("render", &MeshPotato::MPVolume::VDBRayMarcher::render)
		.def("writeImage", &MeshPotato::MPVolume::VDBRayMarcher::writeImage)
	;

	def("getVDBPtr", &getVDBPtr);	
	def("makeVDBGrid", &MeshPotato::MPVolume::makeVDBGrid);
	def("getMPVolumeFromPyObject", &getMPVolumeFromPyObject);
	def("CreateVDBFrustumLight", &CreateVDBFrustumLight);
	def("RenderVDB", &RenderVDB);
	def("ConvertToFogVolume", &ConvertToFogVolume);
}
