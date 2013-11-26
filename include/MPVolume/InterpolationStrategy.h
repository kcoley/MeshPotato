#include "MPUtils/Vector.h"
#include "MPVolume/MPVolume.h"
using MeshPotato::MPUtils::MPVec3;
namespace MeshPotato {
namespace MPVolume {
template <typename T>
class InterpolationStrategy {
public:
	virtual const T eval(const MPVec3 &P) const = 0;
private:
};
template <typename T>
class TrilinearInterpolationStrategy: public InterpolationStrategy<T> {
public:
	TrilinearInterpolationStrategy() {}
	//! Implements Trilinear Interpolation on a Volume Grid
	virtual const T eval(const MPUtils::MPVec3 &P) const {
		MPVec3 p = P - volumeGrid->llc/volumeGrid->delta; 	
		MPVec3 lowerLeft(floor(p.x()), floor(p.y()), floor(p.z()));
		if (lowerLeft.x() >= volumeGrid->llc.x() && (lowerLeft.y() >= volumeGrid->llc.z()) && (lowerLeft.z() >= volumeGrid->llc.z())) {
			float weight[3];
			T value = 0.0;
			for (int i = 0; i < 2; ++i) {
				int cur_x = lowerLeft.x() + i;
				weight[0] = 1.0f - abs(p.x() - cur_x);
				for (int j = 0; j < 2; ++j) {
					int cur_y = lowerLeft.y() + j;
					weight[1] = 1.0f - abs(p.y() - cur_y);
					for (int k = 0; k <= 1; ++k) {
						int cur_z = lowerLeft.z() + k;
						weight[2] = 1.0f - abs(p.z() - cur_z);
						if ((cur_x >= volumeGrid->Nx - 1) || (cur_y > volumeGrid->Ny - 1) || (cur_z > volumeGrid->Nz - 1))
							return volumeGrid->defaultValue();
						if ((cur_x < 0) || (cur_y < 0) || (cur_z < 0))
							return volumeGrid->defaultValue();
						int index = volumeGrid->getIndex(cur_x, cur_y, cur_z);
						value = value + weight[0] * weight[1] * weight[2] * volumeGrid->getValFromIndex(index);
					}

				}
			}
			return value;


		}
		else { return volumeGrid->defaultValue(); }

	}
private:
MeshPotato::MPVolume::Volume<T> *volumeGrid;
};
}
}
