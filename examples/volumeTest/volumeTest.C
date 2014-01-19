#include <MPVolume/MPVolume.h>
#include <MPVolume/VDBVolumeGrid.h>
#include <MPVolume/FieldAlgebra.h>
#include <iostream>
#include <openvdb/openvdb.h>
#include <MPVolume/VDBVolumeUtils.h>
#include <openvdb/tools/LevelSetSphere.h>
#include <MPVolume/ImplicitShape.h>
#include <MPVolume/Transformation.h>
#include <MPVolume/RayMarcher.h>
#include <MPUtils/Image.h>
#include <MPUtils/Camera.h>
#include <MPUtils/OIIOFiles.h>
#include <openvdb/tools/RayTracer.h>
int main() {
	std::cout << "Printing" << std::endl;
	MeshPotato::MPVolume::VolumeFloatPtr grid;

	openvdb::FloatGrid::Ptr  vdbgrid = openvdb::FloatGrid::Ptr();

	openvdb::FloatGrid::Ptr sphere = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(20, openvdb::Vec3f(10, 0, 0), 0.05, 4.0);
	openvdb::FloatGrid::Ptr sphere2 = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(20, openvdb::Vec3f(-20, 0, 0), 0.75, 4.0);

	sphere->setBackground(0);
	sphere2->setBackground(0);

	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > grid4 = MeshPotato::MPVolume::VDBVolumeGrid::Ptr(vdbgrid);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > grid5 = MeshPotato::MPVolume::VDBVolumeGrid::Ptr(sphere);
		
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > mysphere = MeshPotato::MPVolume::ImplicitSphere::Ptr(10, MPVec3(0,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::ImplicitSphere> mysphere2 = MeshPotato::MPVolume::ImplicitSphere::Ptr(10, MPVec3(50,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > clamp_sphere1 = MeshPotato::MPVolume::Clamp<float>::Ptr(mysphere, 0, 100);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > clamp_sphere2 = MeshPotato::MPVolume::Clamp<float>::Ptr(mysphere2, 0, 100);
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > translateSphere = MeshPotato::MPVolume::TranslateVolume<float>::Ptr(mysphere, MeshPotato::MPUtils::MPVec3(30,0,0));
	boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > add = MeshPotato::MPVolume::AddVolume<float, MeshPotato::MPVolume::Volume<float> >::Ptr(clamp_sphere1, clamp_sphere2);
	MeshPotato::MPVolume::VolumeFloatPtr add2 = MeshPotato::MPVolume::AddVolume<float, MeshPotato::MPVolume::Volume<float> >::Ptr(clamp_sphere1, clamp_sphere2);

	grid = add2;
	openvdb::CoordBBox indexBB(openvdb::Coord(-90,-90,-90), openvdb::Coord(90,90,90));
	openvdb::FloatGrid::Ptr newvdbgrid = MeshPotato::MPVolume::makeVDBGrid(add, indexBB, 1.0f);
	
//	MeshPotato::MPVolume::RayMarcher marcher1(grid5, add2, 0.5f, 1.0f);
	MeshPotato::MPVolume::VDBRayMarcher marcher(newvdbgrid, add2, 0.1f, 1.0f);
	std::cout << "marching" << std::endl;
//	MeshPotato::MPUtils::MPRay ray(MeshPotato::MPUtils::MPVec3(0,0,0), MeshPotato::MPUtils::MPVec3(1,1,1));
//   	MeshPotato::MPUtils::Color color = marcher.L(ray);	
//	std::cout << "done marching" << std::endl;
//	std::cout << "[" << color[0] << ", " << color[1] << ", " << color[2] << ", " << color[3] <<  "]" << std::endl;

	int imageWidth = 960;
	int imageHeight = 540;
	MeshPotato::MPUtils::Image image;
	MeshPotato::MPUtils::Camera cam;
	openvdb::tools::Film film(imageWidth, imageHeight);
	openvdb::tools::PerspectiveCamera vdbcam(film, MeshPotato::MPUtils::MPVec3(500,500,500));
//	vdbcam.lookAt(MeshPotato::MPUtils::MPVec3(0,0,0));
	cam.setEyeViewUp(MeshPotato::MPUtils::MPVec3(0.1, 0.1, 0.1), MeshPotato::MPUtils::MPVec3(0,0,1), MeshPotato::MPUtils::MPVec3(0,1,0));
	image.reset(imageWidth, imageHeight);
	for (int j = 0; j < imageHeight; ++j) {
		for (int i = 0; i < imageWidth; ++i) {
			double x = (double)i/(imageWidth - 1);
			double y = (double)j/(imageHeight - 1);
			MeshPotato::MPUtils::MPRay ray = vdbcam.getRay(i,j);
//			std::cout << ray << std::endl;
			setPixel(image, i, j, marcher.L(ray));
//			setPixel(image, i, j, Color(1,0,0,1));
		}
	}
	MeshPotato::MPUtils::writeOIIOImage("test.exr", image);
	openvdb::io::File file("test3.vdb");
	openvdb::GridPtrVec grids;
	grids.push_back(newvdbgrid);
	file.write(grids);
	file.close();
	return 0;
}
