#ifndef __MPVOLUME_H__
#define __MPVOLUME_H__
#include "MPUtils/Vector.h"
#include "MPUtils/Color.h"
#include <boost/shared_ptr.hpp>
namespace MeshPotato {
	//! Handles Volumetric Operations
	namespace MPVolume {
		template<typename T>
			struct GradType {
				typedef int GType;
			};
		template<>
			struct GradType<float> {
				typedef MeshPotato::MPUtils::MPVec3 GType;
			};
		template<>
			struct GradType<MeshPotato::MPUtils::MPVec3> {
				typedef MeshPotato::MPUtils::MPMat3 GType;
			};
		template < typename T>
			class Volume {
				public:
					typedef boost::shared_ptr<Volume<T> > Ptr;
					Volume() {}
					virtual ~Volume() {}
					typedef T volumeDataType;
					typedef typename GradType<T>::GType volumeGradType;
					virtual const volumeDataType eval(const MeshPotato::MPUtils::MPVec3& P) const = 0;
					virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const = 0;
			};

	typedef boost::shared_ptr<MeshPotato::MPVolume::Volume<float> > VolumeFloatPtr;
	typedef boost::shared_ptr<MeshPotato::MPVolume::Volume<MeshPotato::MPUtils::Color> > VolumeColorPtr;
	typedef boost::shared_ptr<MeshPotato::MPVolume::Volume<MeshPotato::MPUtils::MPVec3> > VolumeVectorPtr;
	typedef boost::shared_ptr<MeshPotato::MPVolume::Volume<MeshPotato::MPUtils::MPMat3> > VolumeMatrixPtr;
	}
}
#endif // __MPVOLUME_H__
