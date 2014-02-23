#ifndef __RAYMARCHER_H__
#define __RAYMARCHER_H__
#include "ImplicitShape.h"
#include <iostream>
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>
#include <MPVolume/FrustumGrid.h>
#include "MPUtils/DeepImage.h"
using namespace MeshPotato::MPUtils;
namespace MeshPotato {
namespace MPVolume {
class VDBRayMarcher {
public:
VDBRayMarcher(openvdb::FloatGrid::Ptr _grid, VolumeColorPtr _dsm, const double &_step, const double &_K) : grid(_grid), dsm(_dsm), step(_step), K(_K), interpolator(grid->constTree(), grid->transform()), intersector(*grid) {
}
MeshPotato::MPUtils::Color L(MPRay &ray) {
	Color _L = Color(0,0,0,0);
	float deltaT, deltaS;
	float _T = 1.0f;
	float time = 0.0f;	
	float steptime = 0.0f;	



	if (!intersector.setWorldRay(ray)) return Color(0,0,0,0);
	double t0 = 0, t1 = 0;
	while (int n = intersector.march(t0, t1)) {
		if (time < t0)
			time = t0;
		while (time < t1) {
				MPVec3 P = intersector.getWorldPos(time);
				float density = interpolator.wsSample(P);
				if (n == 2) {// leaf node
					deltaS = step;
				}
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

	                CS.set(CI.X()*CM.Y(),CI.Y()*CM.Y(),CI.Z()*CM.Z(),CM.W());
	            
				//	Color CS = density * (Color(1.0, 1.0, 1.0, 1.0));
					_L += ((CS)*_T*(1 - deltaT));
					_T *=deltaT;
					if (_T < 0.0000001) return Color(_L[0],_L[1],_L[2],1.0 - _T);
				}
				time += deltaS;

		}

	}
	return Color(_L[0],_L[1],_L[2],1.0 - _T);
}
MeshPotato::MPUtils::DeepPixelBuffer deepL(MPRay &ray, MeshPotato::MPUtils::Camera &cam) {
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
private:
openvdb::FloatGrid::Ptr grid;
VolumeColorPtr dsm;
double K, step;
openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator;
openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> intersector;
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
