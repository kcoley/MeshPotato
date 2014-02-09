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
int main(int argc, char **argv) {
lux::CmdLineFind clf(argc, argv);
std::string vdb_volumeFile = clf.find(std::string("-vdb"), std::string("bunny_cloud/bunny_cloud.vdb"), std::string("VDB Fog Volume File"));
std::string outputImage = clf.find(std::string("-name"), std::string("test.exr"), std::string("Name of output image"));
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

MeshPotato::MPUtils::MPVec3 fcam_pos = clf.find(std::string("-feye"), MeshPotato::MPUtils::MPVec3(0.0,18.0,0.0), std::string("Frustum Position"));
MeshPotato::MPUtils::MPVec3 fcam_rot = clf.find(std::string("-frot"), MeshPotato::MPUtils::MPVec3(0.0,0.0,180.0), std::string("Frustum Rotation"));
float fnearP = clf.find("-fnear", 20.0f, "Frustum Near Plane");
float ffarP = clf.find("-ffar", 60.0f, "Frustum Far Plane");

clf.usage("-h");
clf.printFinds();
openvdb::initialize();
cam_rot/= 180.0;
cam_rot.normalize();
	MeshPotato::MPVolume::VolumeFloatPtr grid;
openvdb::GridBase::Ptr baseGrid;
                        openvdb::FloatGrid::Ptr grid_i;
                        openvdb::GridPtrVecPtr grids_i(new openvdb::GridPtrVec);
                        static string sName("Loading VDB ");
                        openvdb::io::File file_i(vdb_volumeFile);
                        file_i.open();
                        openvdb::io::File::NameIterator nameIter = file_i.beginName();
                        baseGrid = file_i.readGrid(nameIter.gridName());




	openvdb::FloatGrid::Ptr inputGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);
	openvdb::FloatGrid::Ptr sphere = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(10, openvdb::Vec3f(0, 0, 0), 0.05);
	openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(*sphere);	

	sphere->setBackground(-1);

	std::cout << "before marching" << std::endl;
	MeshPotato::MPVolume::VolumeFloatPtr grid4 = MeshPotato::MPVolume::VDBVolumeGrid::Ptr(sphere);
	std::cout << "before marching" << std::endl;
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > grid5 = MeshPotato::MPVolume::VDBVolumeGrid::Ptr(inputGrid);
		
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > mysphere = MeshPotato::MPVolume::ImplicitSphere::Ptr(10, MPVec3(0,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > mysphere2 = MeshPotato::MPVolume::ImplicitSphere::Ptr(10, MPVec3(50,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > clamp_sphere1 = MeshPotato::MPVolume::Clamp<float>::Ptr(mysphere, 0, 100);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > clamp_sphere2 = MeshPotato::MPVolume::Clamp<float>::Ptr(mysphere2, 0, 100);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > translateSphere = MeshPotato::MPVolume::TranslateVolume<float>::Ptr(mysphere, MeshPotato::MPUtils::MPVec3(30,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > add = MeshPotato::MPVolume::AddVolume<float, MeshPotato::MPVolume::Volume<float> >::Ptr(clamp_sphere1, clamp_sphere2);
	MeshPotato::MPVolume::VolumeFloatPtr add2 = MeshPotato::MPVolume::AddVolume<float, MeshPotato::MPVolume::Volume<float> >::Ptr(clamp_sphere1, clamp_sphere2);
	float val = grid4->eval(MeshPotato::MPUtils::MPVec3(0,0,0));
	
	std::cout << "val = " << val << std::endl;
	grid = add2;
	openvdb::CoordBBox indexBB(openvdb::Coord(-90,-90,-90), openvdb::Coord(90,90,90));
	openvdb::FloatGrid::Ptr newvdbgrid = MeshPotato::MPVolume::makeVDBGrid(add, indexBB, 1.0f);
//	openvdb::FloatGrid::Ptr blahGrid = sphere.distGridPtr();
//	openvdb::tools::sdfToFogVolume<openvdb::FloatGrid>(*sphere);	
	std::cout << "before marching" << std::endl;
//	MeshPotato::MPVolume::VDBRayMarcher marcher(inputGrid, grid5,stepSize, scattering);
//	MeshPotato::MPVolume::RayMarcher marcher(grid5, grid5,0.5f, 100.0f);
	std::cout << "marching" << std::endl;
//	MeshPotato::MPUtils::MPRay ray(MeshPotato::MPUtils::MPVec3(0,0,0), MeshPotato::MPUtils::MPVec3(1,1,1));
//   	MeshPotato::MPUtils::Color color = marcher.L(ray);	
//	std::cout << "done marching" << std::endl;
//	std::cout << "[" << color[0] << ", " << color[1] << ", " << color[2] << ", " << color[3] <<  "]" << std::endl;

//	int imageWidth = 1920;
//	int imageHeight = 1080;
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

	openvdb::tools::Film film(imageWidth, imageHeight);
//	openvdb::tools::PerspectiveCamera vdbcam(film, MeshPotato::MPUtils::MPVec3(-29,-77,0), MeshPotato::MPUtils::MPVec3(-100,60,23));
	openvdb::tools::PerspectiveCamera vdbcam(film, cam_rot, cam_pos);
	MeshPotato::MPUtils::ProgressMeter meter(imageHeight, "volume render");
//	vdbcam.lookAt(MeshPotato::MPUtils::MPVec3(0,0,0));
openvdb::BBoxd bbox(MeshPotato::MPUtils::MPVec3(0,0,0), MeshPotato::MPUtils::MPVec3(100,100,100));	
	boost::shared_ptr<MeshPotato::MPUtils::Camera> frustumCam = MeshPotato::MPUtils::Camera::Ptr();
	frustumCam->setEyeViewUp(fcam_pos, fcam_rot, MPVec3(0,1,0));
	frustumCam->setNearPlane(fnearP);
	frustumCam->setFarPlane(ffarP);
	frustumCam->setAspectRatio(1.0);
	frustumCam->setFov(90);
	//MeshPotato::MPVolume::FrustumGrid frustum(frustumCam, bbox);
	boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid > frustum = MeshPotato::MPVolume::FrustumGrid::Ptr(frustumCam, bbox);
	openvdb::Coord ijk(50,50,50);
	std::cout << frustum->indexToWorld(ijk) << std::endl;
	std::cout << "dsm" << std::endl;
	frustum->dsm(bunnydsm, dsmK);
	//frustum->dsm(inputGrid, dsmK);
	std::cout << "bunnydsm " << bunnydsm->eval(MeshPotato::MPUtils::MPVec3(0,0,0)) << std::endl;
	std::cout << frustum->indexToWorld(openvdb::Coord(0,0,0)) << std::endl;
	std::cout << frustum->indexToWorld(openvdb::Coord(100,100,500)) << std::endl;
	//return 0;

	MeshPotato::MPVolume::VDBRayMarcher marcher(inputGrid, frustum, stepSize, scattering);
	std::cout << "Marching..." << std::endl;
	image.reset(imageWidth, imageHeight);
	deepimage.reset(imageWidth, imageHeight);
	for (int j = 0; j < imageHeight; ++j) {
		for (int i = 0; i < imageWidth; ++i) {
			double x = (double)i/(imageWidth - 1.0);
			double y = (double)j/(imageHeight - 1.0);
			MeshPotato::MPUtils::MPRay ray = cam.getRay(x,y);//vdbcam.getRay(i,j);//cam.getRay(x,y);
			
//			MeshPotato::MPUtils::MPVec3 vec0 = ray.dir();
//			std::cout << "vec0 = "<< vec0 << std::endl;
	//		std::cout << "1 = " << ray << std::endl;
			
	//		ray = cam.getRay(x,y);//cam.getRay(x,y);
	//		ray = frustumCam->getRay(x,y);//cam.getRay(x,y);

//			MeshPotato::MPUtils::MPVec3 vec = ray.dir();
//			std::cout << "vec = "<< vec << std::endl;
//			MeshPotato::MPUtils::MPVec3 vec2(0,0,1);
//			vec.x() *= -0.41;
//			ray.setDir(vec0);
	//		std::cout << "2 = " << ray << std::endl;

//			ray = cam.getRay(x,y);
//			std::cout << "2: " << ray << std::endl;	
//			ray.setMinTime(80);
//			ray.setMaxTime(120);
//			std::cout << ray << std::endl;
//			std::cout << "pixel = " << i << ", " << j << std::endl;
//			setPixel(image, i, j, marcher.L(ray));
			setPixel(deepimage, i, j, marcher.deepL(ray));
//			setPixel(image, i, j, Color(1,0,0,1));
		}
		meter.update();
	}
	std::cout << "Done Marching" << std::endl;
//	MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), image);
	MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), deepimage);
	openvdb::io::File file("sphere2.vdb");
	openvdb::GridPtrVec grids;
	grids.push_back(sphere);
//	grids.push_back(newvdbgrid);
	file.write(grids);
	file.close();
	clf.printFinds();
	return 0;
}
