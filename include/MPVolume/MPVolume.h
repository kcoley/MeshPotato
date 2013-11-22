#include "MPUtils/Vector.h"
namespace MeshPotato {

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
class MPVolume {
	public:
	MPVolume() {}
	virtual ~MPVolume();
	typedef T volumeDataType;
	typedef typename GradType<T>::GType volumeGradType;
	virtual const volumeDataType eval(const MeshPotato::MPUtils::MPVec3& P) const = 0;
	virtual const volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const = 0;

};

typedef MPVolume<float>* MPVolumeFloatPtr;
//typedef MPVolume<Color>* VolumeColorPtr;
typedef MPVolume<MeshPotato::MPUtils::MPVec3>* MPVolumeVectorPtr;
typedef MPVolume<MeshPotato::MPUtils::MPMat3>* MPVolumeMatrixPtr;

}
