#ifndef __MPVOLUME_H__
#define __MPVOLUME_H__
#include "MPUtils/Vector.h"
namespace MeshPotato {
//! Handles Volumetric Operations on Meshes
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
	Volume() {}
	virtual ~Volume() {}
	typedef T volumeDataType;
	typedef typename GradType<T>::GType volumeGradType;
	virtual const volumeDataType eval(const MeshPotato::MPUtils::MPVec3& P) const = 0;
	virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const = 0;

};

typedef Volume<float>* VolumeFloatPtr;
//typedef MPVolume<Color>* VolumeColorPtr;
typedef Volume<MeshPotato::MPUtils::MPVec3>* VolumeVectorPtr;
typedef Volume<MeshPotato::MPUtils::MPMat3>* VolumeMatrixPtr;
}
}
#endif // __MPVOLUME_H__
