#ifndef __RAYMARCHER_H__
#define __RAYMARCHER_H__
#include "ImplicitShape.h"
#include <iostream>
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>
#include <MPVolume/FrustumGrid.h>
#include "MPUtils/DeepImage.h"
#include "MPUtils/Image.h"
#include "tbb/tbb.h"
#include "MPUtils/OIIOFiles.h"
using namespace MeshPotato::MPUtils;
namespace MeshPotato {
namespace MPVolume {
class VDBRayMarcher {
public:
VDBRayMarcher(openvdb::FloatGrid::Ptr _grid, VolumeColorPtr _dsm, const double &_step, const double &_K, boost::shared_ptr<MeshPotato::MPUtils::Image> _image, boost::shared_ptr<MeshPotato::MPUtils::Camera> _camera, const std::string &_outputImage) : grid(_grid), dsm(_dsm), step(_step), K(_K), interpolator(grid->constTree(), grid->transform()), intersector(*grid), image(_image), camera(_camera), outputImage(_outputImage) {

}
inline const MeshPotato::MPUtils::Color L(MPRay &ray) const {
openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> intersector2(*grid);
	Color _L = Color(0,0,0,0);
	float deltaT, deltaS;
	float _T = 1.0f;
	float time = 0.0f;	
	float steptime = 0.0f;	

	if (!intersector2.setWorldRay(ray)) return Color(0,0,0,0);
	double t0 = 0, t1 = 0;
	while (int n = intersector2.march(t0, t1)) {
		if (time < t0)
			time = t0;
		while (time < t1) {
				MPVec3 P = intersector2.getWorldPos(time);
				float density = interpolator.wsSample(P);
				if (n == 2) {// leaf node
					deltaS = step;
				}
				else // constant tile
					deltaS = step;//t1 - time;
				if (density > 0) {
					deltaT = exp(-K*density*deltaS);
					Color CM(1,1,1,1);
			                Color CI = dsm->eval(P);
			                Color CS = CI*CM;
	            
					_L += ((CS)*_T*(1 - deltaT));
					_T *=deltaT;
					if (_T < 0.0000001) return Color(_L[0],_L[1],_L[2],1.0 - _T);
				}
				time += deltaS;

		}

	}
	return Color(_L[0],_L[1],_L[2],1.0 - _T);
}
const MeshPotato::MPUtils::DeepPixelBuffer deepL(MPRay &ray, MeshPotato::MPUtils::Camera &cam) {
//	Color _L = Color(0,0,0,0);
	float deltaT, deltaS;
	float time = 0.0f;	
	float steptime = 0.0f;	
	MeshPotato::MPUtils::DeepPixelBuffer deepPixelBuf;

	if (!intersector.setWorldRay(ray)) return deepPixelBuf;
	double t0 = 0, t1 = 0;
	while (int n = intersector.march(t0, t1)) {
		if (time < t0)
			time = t0;
		while (time < t1) {
			Color _L = Color(0,0,0,0);
				MPVec3 P = intersector.getWorldPos(time);
				float density = interpolator.wsSample(P);
				if (n == 2) // leaf node
					deltaS = step;
				else // constant tile
					deltaS = step;//t1 - time;
				if (density > 0) {
					deltaT = exp(-K*density*deltaS);
					Color CS,CI,CM;
					CM.set(1,1,1,1);
				//	Color CS = density * (Color(1.0, 1.0, 1.0, 1.0));
	                      //          Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(-dsm->eval(P) * K));
	             //   Color CS = (Color(1.0, 1.0, 1.0, 1.0) * exp(-dsm->eval(P) * K)); 
//	                CI = (Color(1.0, 1.0, 1.0, 1.0) * exp(-dsm->eval(P)));
	                CI = dsm->eval(P);
			MeshPotato::MPUtils::DeepPixel deepPixel;
//	                double test = exp(-dsm->eval(P));

	                CS.set(CI.X()*CM.Y(),CI.Y()*CM.Y(),CI.Z()*CM.Z(),CM.W());
	            
				//	Color CS = density * (Color(1.0, 1.0, 1.0, 1.0));
					_L += ((CS)*(1 - deltaT));
					deepPixel.color = _L;
					deepPixel.color[3] = 1.0 - deltaT;
					deepPixel.depth_front = (P - cam.eye()).length();//(ray(time)).dot(cam.view());
//					std::cout << "before push back" << std::endl;
					deepPixelBuf.push_back(deepPixel);
//					std::cout << "pushed back" << std::endl;
				}
				time += deltaS;
		}

	}
	return deepPixelBuf;
}
void operator() (const tbb::blocked_range<size_t>& r) const {
	for (int j = r.begin(), je = r.end(); j < je; ++j) {
		for (int i = 0, ie = image->Width(); i < ie; ++i) {
			MeshPotato::MPUtils::MPRay ray;
			double x = (double)i/(image->Width() - 1.0);
               		double y = (double)j/(image->Height() - 1.0);
			ray = camera->getRay(x,y);
			Color c = L(ray);
			float &val1 = image->value(i,j,0);
			val1 =c[0]; 
			float &val2 = image->value(i,j,1);
			val2 = c[1];
			float &val3 = image->value(i,j,2);
			val3 = c[2];
			float &val4 = image->value(i,j,3);
			val4 = c[3];
		}
	}
	MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), *image);
}
inline void render(bool threaded) const {
	tbb::blocked_range<size_t> range(0, image->Height());
	threaded ? tbb::parallel_for(range, *this) : (*this)(range);
}
private:
openvdb::FloatGrid::Ptr grid;
VolumeColorPtr dsm;
double K, step;
openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator;
openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> intersector;
boost::shared_ptr<MeshPotato::MPUtils::Image> image;
boost::shared_ptr<MeshPotato::MPUtils::Camera> camera;
std::string outputImage;
};

class RayMarcher {
public:
RayMarcher(VolumeFloatPtr _grid, VolumeColorPtr _dsm, const float &_step, const float &_K) : grid(_grid), dsm(_dsm), step(_step), K(_K)  {}
MeshPotato::MPUtils::Color L(MPRay &ray) {
        Color _L = Color(0,0,0,0);
        float deltaT;
        float _T = 1.0f;
        float time = 0.0f;
        double t0 = ray.t0(), t1 = ray.t1();
	time = t0;
        while (time < t1) {
                        time += step;
                        while (time < t1) {
                        MeshPotato::MPUtils::MPVec3 P = ray(time);
                        float density = grid->eval(P);
                        if (density > 0) {
                                deltaT = exp(-K * density * step);
                                Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(0.0f * K));
//                              Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(-dsm->eval(P) * K));
                                _L += ((CS) * _T * (1 - deltaT));
                                _T *=deltaT;
                                if (_T < 0.01) break;
                        }
                        time += step;
                	}
        }
        return Color(_L[0],_L[1],_L[2],1.0 - _T);
}
private:
MeshPotato::MPVolume::VolumeFloatPtr grid;
VolumeColorPtr dsm;
float K, step;
};




}
}
#endif // __RAYMARCHER_H__
