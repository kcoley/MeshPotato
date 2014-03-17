#ifndef __MPVOLUMEGRID_H__
#define __MPVOLUMEGRID_H__
#include "MPVolume.h"
using MeshPotato::MPUtils::MPVec3;
namespace MeshPotato {
	namespace MPVolume {
	template<typename T>
	class VolumeGrid: public Volume<T> {
	public:
		VolumeGrid() {}
		static boost::shared_ptr<VolumeGrid<T> > Ptr() { return boost::shared_ptr<VolumeGrid<T> >(new VolumeGrid<T>); }
		virtual ~VolumeGrid() {}
		// Should use strategy pattern to be able to 
		// Change the interpolation scheme
		typedef T volumeDataType;
	        typedef typename GradType<T>::GType volumeGradType;

		virtual const volumeDataType eval(const MPUtils::MPVec3 &P) const = 0;
		const T defaultValue() const = 0;
		virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const = 0;
	};
	}
}
#endif // __MPVOLUMEGRID_H__
