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
		typedef T volumeDataType;
	        typedef typename GradType<T>::GType volumeGradType;

		virtual const volumeDataType eval(const MPUtils::MPVec3 &P) const{};
		const T defaultValue() const {};
		virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const {};
	private:
		std::vector<InterpolationStrategy<T> > interpolationStrategy;
		MPVec3 llc, urc;
	};
	}
}
#endif // __MPVOLUMEGRID_H__
