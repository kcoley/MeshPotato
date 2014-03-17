#include "MPVolume/FieldAlgebra.h" 
#include "MPUtils/Color.h"
#include "MPUtils/Vector.h"
namespace MeshPotato {
	namespace MPVolume {
template <typename T>
class AddVolume<T>::Impl {
	public:
		boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f1,f2;
	};
template <typename T>
AddVolume<T>::AddVolume(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) : mImpl(new AddVolume<T>::Impl) {
	mImpl->f1 = _f1;
	mImpl->f2 = _f2;
}
	
template <typename T>
boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > AddVolume<T>::Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) {
	return boost::shared_ptr<Volume<T> >(new AddVolume<T>(_f1, _f2));
}

template <typename T>
const typename Volume<T>::volumeDataType AddVolume<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return mImpl->f1->eval(P) + mImpl->f2->eval(P);
}
template <typename T>
const typename Volume<T>::volumeGradType AddVolume<T>::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}


template class AddVolume<float>;
template class AddVolume<MPUtils::MPVec3>;
template class AddVolume<MPUtils::Color>;
}
}
