#include <MPVolume/MPVolume.h>
#include <MPVolume/VDBVolumeGrid.h>
#include <MPVolume/FieldAlgebra.h>
#include <iostream>
#include <openvdb/openvdb.h>
#include <MPVolume/VolumeUtils.h>
#include <openvdb/tools/LevelSetSphere.h>
#include <openvdb/tools/LevelSetUtil.h>
#include <MPVolume/ImplicitShape.h>
#include <MPVolume/Transformation.h>
#include <MPVolume/RayMarcher.h>
#include <MPUtils/Image.h>
#include <MPUtils/DeepImage.h>
#include <MPUtils/Camera.h>
#include <MPUtils/OIIOFiles.h>
#include <MPUtils/ProgressMeter.h>
#include <openvdb/tools/RayTracer.h>
#include <MPVolume/FrustumGrid.h>
#include <MPUtils/CmdLineFind.h>
#include <limits>
#include <MPVolume/Light.h>
using MeshPotato::MPVolume::VolumeFloatPtr;

int main(int argc, char **argv) {
/////////////////////////Parse Command Line Arguments=================================================
lux::CmdLineFind clf(argc, argv);
std::string vdb_volumeFile = clf.find(std::string("-vdb"), std::string("test.vdb"), std::string("VDB Fog Volume File"));
std::string outputImage = clf.find(std::string("-name"), std::string("test.exr"), std::string("Name of output image"));
int frame = clf.find("-frame", 1, "Frame Number");
int imageWidth = clf.find("-NX", 960, "Image Width");
int imageHeight = clf.find("-NY", 540, "Image Height");
float stepSize = clf.find("-step", 1.0f, "Step size");
float fov = clf.find("-fov", 60.0f, "Field of View");
float nearP = clf.find("-near", 1.0f, "Near Plane");
float farP = clf.find("-far", std::numeric_limits<float>::max(), "Far Plane");
float scattering = clf.find("-K", 1.0f, "Scattering Coefficient");
float dsmK = clf.find("-dsm", 5.0f, "DSM Coefficient");
MeshPotato::MPUtils::MPVec3 cam_pos = clf.find(std::string("-eye"), MeshPotato::MPUtils::MPVec3(0.0,18.0,90.0), std::string("Camera Position"));
MeshPotato::MPUtils::MPVec3 cam_rot = clf.find(std::string("-rot"), MeshPotato::MPUtils::MPVec3(0.0,0.0,-180.0), std::string("Camera Rotation"));

MeshPotato::MPUtils::MPVec3 fcam_pos = clf.find(std::string("-feye"), MeshPotato::MPUtils::MPVec3(0.0,18.0,20.0), std::string("Frustum Position 1"));
MeshPotato::MPUtils::MPVec3 fcam_pos2 = clf.find(std::string("-feye2"), MeshPotato::MPUtils::MPVec3(-30.0,18.0,0.0), std::string("Frustum Position 2"));
MeshPotato::MPUtils::MPVec3 fcam_pos3 = clf.find(std::string("-feye3"), MeshPotato::MPUtils::MPVec3(-30.0,18.0,0.0), std::string("Frustum Position 3"));
//MeshPotato::MPUtils::MPVec3 fcam_rot = clf.find(std::string("-frot"), MeshPotato::MPUtils::MPVec3(0.0,0.0,180.0), std::string("Frustum Rotation"));
float fnearP = clf.find("-fnear", 20.0f, "Frustum Near Plane");
float ffarP = clf.find("-ffar", 60.0f, "Frustum Far Plane");
float frequency = clf.find("-freq", 5.0f, "Frequncy");
float amplitude = clf.find("-amp", 3.0f, "Amplitude");
float roughness = clf.find("-rough", 1.0f, "Roughness");
clf.usage("-h");
clf.printFinds();


openvdb::initialize();
cam_rot/= 180.0;
cam_rot.normalize();
	openvdb::FloatGrid::Ptr sphere = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(10, openvdb::Vec3f(0, 0, 0), 0.05);
	openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(*sphere);	

	sphere->setBackground(-1);

	std::cout << "before marching" << std::endl;
	MeshPotato::MPVolume::VolumeFloatPtr grid4 = MeshPotato::MPVolume::VDBVolumeGrid::Ptr(sphere);
	std::cout << "before marching" << std::endl;
	MeshPotato::MPVolume::VolumeColorPtr basecolor = MeshPotato::MPVolume::ConstantVolume<MeshPotato::MPUtils::Color>::Ptr(MeshPotato::MPUtils::Color(0,0,0,0));		
//	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > mysphere = MeshPotato::MPVolume::ImplicitSphere::Ptr(10, MPVec3(0,0,0));
	MeshPotato::MPNoise::Noise_t noise;
	noise.frequency = frequency;
	noise.amplitude = amplitude;
	noise.roughness = roughness;
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > mysphere = MeshPotato::MPVolume::PyroclasticSphere::Ptr(1, MPVec3(0,0,0), noise);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > mysphere2 = MeshPotato::MPVolume::ImplicitSphere::Ptr(10, MPVec3(50,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > clamp_sphere1 = MeshPotato::MPVolume::Clamp<float>::Ptr(mysphere, 0, 100);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > clamp_sphere2 = MeshPotato::MPVolume::Clamp<float>::Ptr(mysphere2, 0, 100);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > translateSphere = MeshPotato::MPVolume::TranslateVolume<float>::Ptr(mysphere, MeshPotato::MPUtils::MPVec3(30,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > add = MeshPotato::MPVolume::AddVolume<float >::Ptr(clamp_sphere1, clamp_sphere2);
	MeshPotato::MPVolume::VolumeFloatPtr add2 = MeshPotato::MPVolume::AddVolume<float >::Ptr(clamp_sphere1, clamp_sphere2);
	float val = grid4->eval(MeshPotato::MPUtils::MPVec3(0,0,0));
	
	openvdb::CoordBBox indexBB(openvdb::Coord(-200,-200,-200), openvdb::Coord(200,200,200));
	openvdb::FloatGrid::Ptr newvdbgrid = MeshPotato::MPVolume::makeVDBGrid(mysphere, indexBB, 0.05);
//	openvdb::FloatGrid::Ptr newvdbgrid = MeshPotato::MPVolume::makeVDBGrid(clamp_sphere1, indexBB, 0.01f);
	openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(newvdbgrid.operator*());
	openvdb::io::File file(vdb_volumeFile);
	openvdb::GridPtrVec grids;
	grids.push_back(newvdbgrid);
	file.write(grids);
	file.close();
//	return 0;
	MeshPotato::MPVolume::VolumeFloatPtr grid;
	openvdb::GridBase::Ptr baseGrid = MeshPotato::MPVolume::readVDBGrid(vdb_volumeFile);

	openvdb::FloatGrid::Ptr inputGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);
	std::cout << "marching" << std::endl;

	MeshPotato::MPUtils::Image image;
	MeshPotato::MPUtils::DeepImage deepimage;
	MeshPotato::MPUtils::Camera cam;
	std::cout << "cam_rot = " << cam_rot << std::endl;
	std::cout << "cam_rot normalized = " << cam_rot.unit() << std::endl;
	cam.setEyeViewUp(cam_pos, cam_rot, MeshPotato::MPUtils::MPVec3(0,1,0));
	cam.setNearPlane(nearP);
	cam.setFarPlane(farP);
		
	MeshPotato::MPVolume::VolumeFloatPtr bunnydsm = MeshPotato::MPVolume::VDBVolumeGrid::Ptr(inputGrid);
	std::cout << bunnydsm->eval(MeshPotato::MPUtils::MPVec3(0,0,0)) << std::endl;

//	openvdb::tools::Film film(imageWidth, imageHeight);
//	openvdb::tools::PerspectiveCamera vdbcam(film, MeshPotato::MPUtils::MPVec3(-29,-77,0), MeshPotato::MPUtils::MPVec3(-100,60,23));
//	openvdb::tools::PerspectiveCamera vdbcam(film, cam_rot, cam_pos);
	MeshPotato::MPUtils::ProgressMeter meter(imageHeight, "volume render");
//	vdbcam.lookAt(MeshPotato::MPUtils::MPVec3(0,0,0));
openvdb::BBoxd bbox(MeshPotato::MPUtils::MPVec3(0,0,0), MeshPotato::MPUtils::MPVec3(100,100,100));	
	boost::shared_ptr<MeshPotato::MPUtils::Camera> frustumCam = MeshPotato::MPUtils::Camera::Ptr();



//Building Frustum Camera////////////////////////////
frustumCam = MeshPotato::MPVolume::buildFrustumCamera(fcam_pos, inputGrid);
boost::shared_ptr<MeshPotato::MPUtils::Camera> frustumCam2 = MeshPotato::MPVolume::buildFrustumCamera(fcam_pos2, inputGrid);
boost::shared_ptr<MeshPotato::MPUtils::Camera> frustumCam3 = MeshPotato::MPVolume::buildFrustumCamera(fcam_pos3, inputGrid);
////End Building Frustum Camera//////////////////////////////////////////////////////////////



	//MeshPotato::MPVolume::FrustumGrid frustum(frustumCam, bbox);
	boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid > frustum = MeshPotato::MPVolume::FrustumGrid::Ptr(frustumCam, bbox);
	boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid > frustum2 = MeshPotato::MPVolume::FrustumGrid::Ptr(frustumCam2, bbox);
	boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid > frustum3 = MeshPotato::MPVolume::FrustumGrid::Ptr(frustumCam3, bbox);
//	boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid > frustum2 = MeshPotato::MPVolume::FrustumGrid::Ptr(frustumCam, bbox);
//	MeshPotato::MPVolume::VolumeFloatPtr addFrustums = MeshPotato::MPVolume::AddVolume<float>::Ptr(frustum, frustum2);
	openvdb::Coord ijk(50,50,50);
	std::cout << frustum->indexToWorld(ijk) << std::endl;
	std::cout << "dsm" << std::endl;
	frustum->dsm(bunnydsm, dsmK);
	frustum2->dsm(bunnydsm, dsmK);
	frustum3->dsm(bunnydsm, dsmK);
	boost::shared_ptr<MeshPotato::MPVolume::FrustumLight> lightR = MeshPotato::MPVolume::FrustumLight::Ptr(frustum, MeshPotato::MPUtils::Color(1,0,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::FrustumLight> lightG = MeshPotato::MPVolume::FrustumLight::Ptr(frustum2, MeshPotato::MPUtils::Color(0,1,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::FrustumLight> lightB = MeshPotato::MPVolume::FrustumLight::Ptr(frustum3, MeshPotato::MPUtils::Color(0,0,1,0));

	MeshPotato::MPVolume::VolumeColorPtr addLight = MeshPotato::MPVolume::AddVolume<MeshPotato::MPUtils::Color>::Ptr(lightR, lightG);
	MeshPotato::MPVolume::VolumeColorPtr addLights = MeshPotato::MPVolume::AddVolume<MeshPotato::MPUtils::Color>::Ptr(addLight, lightB);
	//frustum->dsm(inputGrid, dsmK);
	std::cout << "bunnydsm " << bunnydsm->eval(MeshPotato::MPUtils::MPVec3(0,0,0)) << std::endl;
	std::cout << frustum->indexToWorld(openvdb::Coord(0,0,0)) << std::endl;
	std::cout << frustum->indexToWorld(openvdb::Coord(100,100,500)) << std::endl;
	//return 0;

//	MeshPotato::MPVolume::VDBRayMarcher marcher(inputGrid, light1, stepSize, scattering);
	MeshPotato::MPVolume::VDBRayMarcher marcher(inputGrid, addLights, stepSize, scattering);
	std::cout << "Marching..." << std::endl;
	image.reset(imageWidth, imageHeight);
	deepimage.reset(imageWidth, imageHeight);
	for (int j = 0; j < imageHeight; ++j) {
		for (int i = 0; i < imageWidth; ++i) {
			double x = (double)i/(imageWidth - 1.0);
			double y = (double)j/(imageHeight - 1.0);
			MeshPotato::MPUtils::MPRay ray = cam.getRay(x,y);//vdbcam.getRay(i,j);//cam.getRay(x,y);

			setPixel(deepimage, i, j, marcher.deepL(ray, cam));
			setPixel(image, i, j, marcher.L(ray));
		}
		meter.update();
	}
	std::cout << "Done Marching" << std::endl;
	MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), image);
	MeshPotato::MPUtils::writeOIIOImage(("deep" + outputImage).c_str(), deepimage);
	clf.printFinds();
	return 0;
}
