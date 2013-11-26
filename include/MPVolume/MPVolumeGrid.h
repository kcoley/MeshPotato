#ifndef __MPVOLUMEGRID_H__
#define __MPVOLUMEGRID_H__
#include "MPVolume.h"
#include "InterpolationStrategy.h"
using MeshPotato::MPUtils::MPVec3;
namespace MeshPotato {
	namespace MPVolume {
	template<typename T>
	class VolumeGrid: public Volume<T> {
	friend class InterpolationStrategy<T>;
	public:
		VolumeGrid() {}
		virtual ~VolumeGrid() {}
		// Should use strategy pattern to be able to 
		// Change the interpolation scheme
		virtual const T eval(const MPUtils::MPVec3 &P) const = 0;
		virtual const T defaultValue() const;
	private:
		std::vector<InterpolationStrategy<T> > interpolationStrategy;
		MPVec3 llc, urc;
	};
	}
}
#endif // __MPVOLUMEGRID_H__
