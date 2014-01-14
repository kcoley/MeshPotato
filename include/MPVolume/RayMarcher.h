#ifndef __RAYMARCHER_H__
#define __RAYMARCHER_H__
#include "ImplicitShape.h"
#include <iostream>
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>

using namespace MeshPotato::MPUtils;
namespace MeshPotato {
namespace MPVolume {
class VDBRayMarcher {
public:
VDBRayMarcher(openvdb::FloatGrid::Ptr _grid, VolumeFloatPtr _dsm, const float &_step, const float &_K) : grid(_grid), dsm(_dsm), step(_step), K(_K), interpolator(grid->constTree(), grid->transform()), intersector(*grid) {
}
MeshPotato::MPUtils::Color L(MPRay &ray) {
	Color _L = Color(0,0,0,0);
	float deltaT;
	float _T = 1.0f;
	float time = 0.0f;	
	intersector.setWorldRay(ray);
	double t0 = 0, t1 = 0;
	while (int n = intersector.march(t0, t1)) {
		if (n == 2) { // leaf node; values are unknown between t0 and t1
			time += step;
			while (time < t1) {
			MPVec3 P = intersector.getWorldPos(time);
			float density = -interpolator.wsSample(P);
			if (density > 0) {
				deltaT = exp(-K * density * step);
				Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(0.0f * K));
//				Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(-dsm->eval(P) * K));
				_L += ((CS) * _T * (1 - deltaT));
				_T *=deltaT;
				if (_T < 0.01) break;
			}
			time += step;
			}
		}
		else { // tile node; values between t0 and t1 are constant
			MPVec3 P = intersector.getWorldPos(time);
			float density = -interpolator.wsSample(P);						
			float deltaS = t1 - t0;
			if (density > 0) {
				deltaT = exp(-K * density * deltaS);
                                Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(0.0f * K));
//                              Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(-dsm->eval(P) * K));
                                _L += ((CS) * _T * (1 - deltaT));
                                _T *=deltaT;
                                if (_T < 0.01) break;
			}
			time += deltaS;
		}
	}
	return Color(_L[0],_L[1],_L[2],1.0 - _T);
}
private:
openvdb::FloatGrid::Ptr grid;
VolumeFloatPtr dsm;
float K, step;
openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::QuadraticSampler> interpolator;
openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> intersector;
};
}
}
#endif // __RAYMARCHER_H__
