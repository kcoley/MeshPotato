#include <boost/python.hpp>
#include <MeshPotato/MPVolume/MPVolume.h>
#include <MeshPotato/MPVolume/VDBVolumeGrid.h>
#include <MeshPotato/MPVolume/FieldAlgebra.h>
#include <MeshPotato/MPVolume/VolumeUtils.h>
#include <MeshPotato/MPVolume/ImplicitShape.h>
#include <MeshPotato/MPVolume/RayMarcher.h>
#include <MeshPotato/MPVolume/FrustumGrid.h>
#include <MeshPotato/MPVolume/Light.h>
#include <MeshPotato/MPVolume/VolumeUtils.h>
#include <MeshPotato/MPVolume/Transformation.h>
#include <MeshPotato/MPUtils/Camera.h>
#include <string>
#include <openvdb/openvdb.h>
#include <openvdb/tools/LevelSetUtil.h>
#include <tbb/task_scheduler_init.h>
#include <MeshPotato/MPVolume/MPVolumeGrid.h>
#include <MeshPotato/MPVolume/DenseGrid.h>
#include <MeshPotato/MPVolume/FFTDivFree.h>
#include <MeshPotato/MPUtils/AttributeTable.h>

using namespace boost::python;
using namespace MeshPotato::MPVolume;
using namespace MeshPotato::MPUtils;

object ConvertToFogVolume(object &grid) {
	openvdb::FloatGrid grid_obj = extract<openvdb::FloatGrid>(grid);
	openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(grid_obj);

	return object(grid_obj);

}
void SetTBBThreads(object numthreads) {
int num_threads = extract<int>(numthreads);
tbb::task_scheduler_init schedulerInit(num_threads);
}
MeshPotato::MPVolume::VolumeColorPtr CreateFrustumLight(MPVec3 light_pos, Color light_color, MPVec3 light_resolution, openvdb::FloatGrid grid, float dsmK=1.0) {
	openvdb::FloatGrid::Ptr gridPtr = grid.copy();
	boost::shared_ptr<MeshPotato::MPUtils::Camera> frustumCam = MeshPotato::MPVolume::buildFrustumCamera(light_pos, gridPtr);
	openvdb::BBoxd bbox(MeshPotato::MPUtils::MPVec3(0,0,0), light_resolution);
	boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid > frustum = MeshPotato::MPVolume::FrustumGrid::Ptr(frustumCam, bbox);
	MeshPotato::MPVolume::VolumeFloatPtr dsmgrid = MeshPotato::MPVolume::VDBVolumeGrid<float>::Ptr(gridPtr);
	frustum->dsm(dsmgrid, dsmK);
	VolumeColorPtr light = MeshPotato::MPVolume::FrustumLight::Ptr(frustum, light_color);
	return light;

};
object CreateVDBFrustumLight(object& light_pos, object& light_color, object& light_resolution, object& grid, object dsmK) {
	extract<openvdb::FloatGrid> grid_obj(grid);
	extract<MPVec3> light_pos_obj(light_pos);
	extract<Color> light_color_obj(light_color);
	extract<MPVec3> light_resolution_obj(light_resolution);
	extract<float> dsm_k_obj(dsmK);
	return object(CreateFrustumLight(light_pos_obj, light_color_obj, light_resolution_obj, grid_obj, dsm_k_obj));

};
void RenderVDBFunc(openvdb::FloatGrid grid, VolumeColorPtr light, double step, double k, boost::shared_ptr<Image> image, boost::shared_ptr<DeepImage> deepimage, boost::shared_ptr<Camera> camera, std::string name, bool threaded) {
	const bool isLevelSet = (grid.getGridClass() == openvdb::GRID_LEVEL_SET);
	openvdb::FloatGrid::Ptr gridPtr = grid.copy();
	if (isLevelSet) {
		//convert to fog volume
		openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(*gridPtr);
	}
	openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> inter(*gridPtr);
	AttributeTable table;
	table.addDoubleAttr("step", step);
	table.addDoubleAttr("K", k);
	MeshPotato::MPVolume::VDBRayMarcher marcher = MeshPotato::MPVolume::VDBRayMarcher(gridPtr, light, camera, table);
	marcher.render(threaded);
	marcher.writeImage(name);
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

object FFTDivFreeFunc(object grid_obj, object field_obj) {
	VolumeGridVectorPtr grid = extract<VolumeGridVectorPtr > (grid_obj);
	VolumeVectorPtr field = extract<VolumeVectorPtr > (field_obj);
	return object(FFTDivFree(grid, field));
}


struct MPVolumeFloatWrap : Volume<float>, wrapper<Volume<float> > {
	const float eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
	const MPVec3 grad(const MPVec3& P) const {
		return this->get_override("grad")();
	}
};

struct MPVolumeGridFloatWrap : VolumeGrid<float>, wrapper<VolumeGrid<float> > {
	const float eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
	void set(const MeshPotato::MPUtils::Coord, const float value) {
		this->get_override("set")();
	}
	const float get(const MeshPotato::MPUtils::Coord) const {
		this->get_override("get")();
	}
	void add(const MeshPotato::MPUtils::Coord, const float value) {
		 this->get_override("add")();
	}
	const MPVec3 grad(const MPVec3& P) const {
		return this->get_override("grad")();
	}
	void stamp(boost::shared_ptr<Volume<float> > volume)  {
		 this->get_override("stamp")();
	}
	const int nx() const {
		 this->get_override("nx")();
	}
	const int ny() const {
		 this->get_override("ny")();
	}
	const int nz() const {
		 this->get_override("nz")();
	}
	const MPVec3 indexToWorld(const MeshPotato::MPUtils::Coord) const {
		return this->get_override("indexToWorld")();
	}
	const MPVec3 worldToIndex(const MPVec3) const {
		return this->get_override("worldToIndex")();
	}
	boost::shared_ptr<VolumeGrid<float> > deepCopy() {
		this->get_override("deepCopy")();
	}
};

struct MPVolumeGridVectorWrap : VolumeGrid<MPVec3>, wrapper<VolumeGrid<MPVec3> > {
	const MPVec3 eval(const MPVec3& P) const {
		return this->get_override("eval")();
	}
	void set(const MeshPotato::MPUtils::Coord, const MPVec3 value) {
		 this->get_override("set")();
	}
	const MPVec3 get(const MeshPotato::MPUtils::Coord) const {
		this->get_override("get")();
	}
	void add(const MeshPotato::MPUtils::Coord, const MPVec3 value) {
		 this->get_override("add")();
	}
	const MPMat3 grad(const MPVec3& P) const {
		return this->get_override("grad")();
	}
	void stamp(boost::shared_ptr<Volume<MPVec3> >)  {
		 this->get_override("stamp")();
	}
	const int nx() const {
		 this->get_override("nx")();
	}
	const int ny() const {
		 this->get_override("ny")();
	}
	const int nz() const {
		 this->get_override("nz")();
	}
	boost::shared_ptr<VolumeGrid<MPVec3> > deepCopy() {
		this->get_override("deepCopy")();
	}
	const MPVec3 indexToWorld(const MeshPotato::MPUtils::Coord) const {
		return this->get_override("indexToWorld")();
	}
	const MPVec3 worldToIndex(const MPVec3) const {
		return this->get_override("worldToIndex")();
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
	class_<boost::shared_ptr<Volume<MPVec3> > >("mpvolume_vector")
	;
	class_<boost::shared_ptr<Volume<Color> > >("mpvolume_color")
	;
	class_<MPVolumeFloatWrap, boost::noncopyable>("VolumeFloat")
		.def("eval", pure_virtual(&Volume<float>::eval))	
		.def("grad", pure_virtual(&Volume<float>::grad))	
	;
	class_<MPVolumeGridFloatWrap, boost::noncopyable>("VolumeGridFloat")
		.def("eval", pure_virtual(&VolumeGrid<float>::eval))	
		.def("set", pure_virtual(&VolumeGrid<float>::set))
		.def("get", pure_virtual(&VolumeGrid<float>::get))	
		.def("add", pure_virtual(&VolumeGrid<float>::add))	
		.def("grad", pure_virtual(&VolumeGrid<float>::grad))
		.def("stamp", pure_virtual(&VolumeGrid<float>::stamp))	
		.def("nx",  pure_virtual(&VolumeGrid<float>::nx))
		.def("ny",  pure_virtual(&VolumeGrid<float>::ny))
		.def("nz",  pure_virtual(&VolumeGrid<float>::nz))
		.def("deepCopy", pure_virtual(&VolumeGrid<float>::deepCopy))
		.def("indexToWorld", pure_virtual(&VolumeGrid<float>::indexToWorld))
		.def("worldToIndex", pure_virtual(&VolumeGrid<float>::worldToIndex))
	;
	class_<MPVolumeGridVectorWrap, boost::noncopyable>("VolumeGridVector")
		.def("eval", pure_virtual(&VolumeGrid<MPVec3>::eval))	
		.def("set", pure_virtual(&VolumeGrid<MPVec3>::set))	
		.def("get", pure_virtual(&VolumeGrid<MPVec3>::get))	
		.def("add", pure_virtual(&VolumeGrid<MPVec3>::add))	
		.def("grad", pure_virtual(&VolumeGrid<MPVec3>::grad))	
		.def("stamp", pure_virtual(&VolumeGrid<MPVec3>::stamp))	
		.def("nx",  pure_virtual(&VolumeGrid<MPVec3>::nx))
		.def("ny",  pure_virtual(&VolumeGrid<MPVec3>::ny))
		.def("nz",  pure_virtual(&VolumeGrid<MPVec3>::nz))
		.def("deepCopy", pure_virtual(&VolumeGrid<MPVec3>::deepCopy))
		.def("indexToWorld", pure_virtual(&VolumeGrid<MPVec3>::indexToWorld))
		.def("worldToIndex", pure_virtual(&VolumeGrid<MPVec3>::worldToIndex))
	;
	class_<MPVolumeVectorWrap, boost::noncopyable>("VolumeVector")
		.def("eval", pure_virtual(&Volume<MPVec3>::eval))	
		.def("grad", pure_virtual(&Volume<MPMat3>::grad))	
	;
	class_<MPVolumeColorWrap, boost::noncopyable>("VolumeColor")
		.def("eval", pure_virtual(&Volume<Color>::eval))	
		.def("grad", pure_virtual(&Volume<Color>::grad))	
	;
	class_<VDBVolumeGrid<float>, boost::shared_ptr<VDBVolumeGrid<float> >, 
			bases<MPVolumeFloatWrap> >("VDBVolumeGrid", no_init)
		.def(init<openvdb::FloatGrid::Ptr>())
		.def("ptr", &VDBVolumeGrid<float>::Ptr)
		.staticmethod("ptr")
		.def("eval", &VDBVolumeGrid<float>::eval)
		.def("grad", &VDBVolumeGrid<float>::grad)
	;
	class_<DenseGrid<float>,  bases<MPVolumeGridFloatWrap> >("DenseGridFloat", no_init)
		.def("__init__", make_constructor(&DenseGrid<float>::Ptr))
		.def("eval", &DenseGrid<float>::eval)
		.def("grad", &DenseGrid<float>::grad)
		.def("set", &DenseGrid<float>::set)
		.def("add", &DenseGrid<float>::add)
		.def("stamp", &DenseGrid<float>::stamp)
		.def("nx", &DenseGrid<float>::nx)
		.def("ny", &DenseGrid<float>::ny)
		.def("nz", &DenseGrid<float>::nz)
		.def("deepCopy", &DenseGrid<float>::deepCopy)
	;
	class_<DenseGrid<MPVec3>,  bases<MPVolumeGridVectorWrap> >("DenseGridVector", no_init)
		.def("__init__", make_constructor(&DenseGrid<MPVec3>::Ptr))
		.def("eval", &DenseGrid<MPVec3>::eval)
		.def("grad", &DenseGrid<MPVec3>::grad)
		.def("set", &DenseGrid<MPVec3>::set)
		.def("add", &DenseGrid<MPVec3>::add)
		.def("stamp", &DenseGrid<MPVec3>::stamp)
		.def("nx", &DenseGrid<MPVec3>::nx)
		.def("ny", &DenseGrid<MPVec3>::ny)
		.def("nz", &DenseGrid<MPVec3>::nz)
		.def("deepCopy", &DenseGrid<MPVec3>::deepCopy)
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
	class_<BlendFloat,  bases<MPVolumeFloatWrap> >("BlendFloat", no_init)
		.def("__init__", make_constructor(&BlendFloat::Ptr))
		.def("eval", &BlendFloat::eval)
		.def("grad", &BlendFloat::grad)
	;
	class_<IntersectionFloat,  bases<MPVolumeFloatWrap> >("IntersectionFloat", no_init)
		.def("__init__", make_constructor(&IntersectionFloat::Ptr))
		.def("eval", &IntersectionFloat::eval)
		.def("grad", &IntersectionFloat::grad)
	;
	class_<UnionVector,  bases<MPVolumeFloatWrap> >("UnionVector", no_init)
		.def("__init__", make_constructor(&UnionVector::Ptr))
		.def("eval", &UnionVector::eval)
		.def("grad", &UnionVector::grad)
	;
	class_<CutoutFloat,  bases<MPVolumeFloatWrap> >("CutoutFloat", no_init)
		.def("__init__", make_constructor(&CutoutFloat::Ptr))
		.def("eval", &CutoutFloat::eval)
		.def("grad", &CutoutFloat::grad)
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
	class_<TranslateVolumeFloat, bases<MPVolumeFloatWrap> >("TranslateVolumeFloat", no_init)
		.def("__init__", make_constructor(&TranslateVolumeFloat::Ptr))
		.def("eval", &MeshPotato::MPVolume::TranslateVolumeFloat::eval)
		.def("grad", &MeshPotato::MPVolume::TranslateVolumeFloat::grad)
	;
	class_<RotateVolumeFloat, bases<MPVolumeFloatWrap> >("RotateVolumeFloat", no_init)
		.def("__init__", make_constructor(&RotateVolumeFloat::Ptr))
		.def("eval", &MeshPotato::MPVolume::RotateVolumeFloat::eval)
		.def("grad", &MeshPotato::MPVolume::RotateVolumeFloat::grad)
	;
	class_<ScaleVolumeFloat, bases<MPVolumeFloatWrap> >("ScaleVolumeFloat", no_init)
		.def("__init__", make_constructor(&ScaleVolumeFloat::Ptr))
		.def("eval", &MeshPotato::MPVolume::ScaleVolumeFloat::eval)
		.def("grad", &MeshPotato::MPVolume::ScaleVolumeFloat::grad)
	;
	class_<VectorNoise,bases<MPVolumeVectorWrap> >("VectorNoise", no_init)
		.def("__init__", make_constructor(&VectorNoise::Ptr))
		.def("eval", &MeshPotato::MPVolume::VectorNoise::eval)
		.def("grad", &MeshPotato::MPVolume::VectorNoise::grad)
	;
	class_<Identity,bases<MPVolumeVectorWrap> >("Identity", no_init)
		.def("__init__", make_constructor(&Identity::Ptr))
		.def("eval", &MeshPotato::MPVolume::Identity::eval)
		.def("grad", &MeshPotato::MPVolume::Identity::grad)
	;
	class_<AdvectVolumeFloat,bases<MPVolumeFloatWrap> >("AdvectVolumeFloat", no_init)
		.def("__init__", make_constructor(&AdvectVolumeFloat::Ptr))
		.def("eval", &MeshPotato::MPVolume::AdvectVolumeFloat::eval)
		.def("grad", &MeshPotato::MPVolume::AdvectVolumeFloat::grad)
	;
	class_<AdvectVolumeVector,bases<MPVolumeVectorWrap> >("AdvectVolumeVector", no_init)
		.def("__init__", make_constructor(&AdvectVolumeVector::Ptr))
		.def("eval", &MeshPotato::MPVolume::AdvectVolumeVector::eval)
		.def("grad", &MeshPotato::MPVolume::AdvectVolumeVector::grad)
	;
	class_<FrustumLight, bases<MPVolumeColorWrap> >("FrustumLight", no_init)
		.def("__init__", make_constructor(&FrustumLight::Ptr))
	;	
	class_<VDBRayMarcher>("VDBRayMarcher", no_init)
		.def(init<openvdb::FloatGrid::Ptr, VolumeColorPtr, boost::shared_ptr<Camera> , MeshPotato::MPUtils::AttributeTable>())
		.def("render", &MeshPotato::MPVolume::VDBRayMarcher::render)
		.def("writeImage", &MeshPotato::MPVolume::VDBRayMarcher::writeImage)
	;
	class_<RayMarcher>("RayMarcher", no_init)
		.def(init<VolumeFloatPtr, VolumeColorPtr, boost::shared_ptr<Camera>, MeshPotato::MPUtils::AttributeTable>())
		.def("render", &MeshPotato::MPVolume::RayMarcher::render)
		.def("writeImage", &MeshPotato::MPVolume::RayMarcher::writeImage)
	;

	def("getVDBPtr", &getVDBPtr, "returns a VDB Pointer");	
	def("makeVDBGrid", &MeshPotato::MPVolume::makeVDBGrid, "Converts a volume into a VDB grid");
	def("getMPVolumeFromPyObject", &getMPVolumeFromPyObject);
	def("CreateVDBFrustumLight", &CreateVDBFrustumLight);
	def("RenderVDB", &RenderVDB);
	def("ConvertToFogVolume", &ConvertToFogVolume);
	def("SetTBBThreads", &SetTBBThreads);
	def("FFTDivFree", &FFTDivFree);
}
