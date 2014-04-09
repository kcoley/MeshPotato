#ifndef __MPVOLUMEGRID_H__
#define __MPVOLUMEGRID_H__
#include "MPVolume.h"
using MeshPotato::MPUtils::MPVec3;
namespace MeshPotato {
	namespace MPVolume {
	template<typename T>
	class VolumeGrid: public Volume<T> {
	public:
		virtual ~VolumeGrid() {}

		virtual const typename Volume<T>::volumeDataType eval(const MPUtils::MPVec3 &P) const = 0;
		virtual const typename Volume<T>::volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const = 0;

	};
	}
}
#endif // __MPVOLUMEGRID_H__
