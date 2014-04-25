#include "MPVolume/FieldAlgebra.h" 
#include "MPUtils/Color.h"
#include "MPUtils/Vector.h"
#include <cmath>
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

template <typename T>
class Union<T>::Impl {
	public:
		boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f1,f2;
	};
template <typename T>
Union<T>::Union(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) : mImpl(new Union<T>::Impl) {
	mImpl->f1 = _f1;
	mImpl->f2 = _f2;
}
	
template <typename T>
boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Union<T>::Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) {
	return boost::shared_ptr<Volume<T> >(new Union<T>(_f1, _f2));
}

template <typename T>
const typename Volume<T>::volumeDataType Union<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return std::max(mImpl->f1->eval(P), mImpl->f2->eval(P));
}
template <typename T>
const typename Volume<T>::volumeGradType Union<T>::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}


template class AddVolume<float>;
template class AddVolume<MPUtils::MPVec3>;
template class AddVolume<MPUtils::Color>;

template class Union<float>;
template class Union<MPUtils::MPVec3>;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class Blend<T>::Impl {
	public:
		boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f1,f2;
		float A;
	};
template <typename T>
Blend<T>::Blend(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2, const float _A) : mImpl(new Blend<T>::Impl) {
	mImpl->f1 = _f1;
	mImpl->f2 = _f2;
	mImpl->A = _A;
}
	
template <typename T>
boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Blend<T>::Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2, const float _A) {
	return boost::shared_ptr<Volume<T> >(new Blend<T>(_f1, _f2, _A));
}

template <typename T>
const typename Volume<T>::volumeDataType Blend<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return exp(mImpl->f1->eval(P)) + exp( mImpl->f2->eval(P)) - mImpl->A;
}
template <typename T>
const typename Volume<T>::volumeGradType Blend<T>::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}


template class Blend<float>;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Intersection<T>::Impl {
	public:
		boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f1,f2;
		float A;
	};
template <typename T>
Intersection<T>::Intersection(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) : mImpl(new Intersection<T>::Impl) {
	mImpl->f1 = _f1;
	mImpl->f2 = _f2;
}
	
template <typename T>
boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Intersection<T>::Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) {
	return boost::shared_ptr<Volume<T> >(new Intersection<T>(_f1, _f2));
}

template <typename T>
const typename Volume<T>::volumeDataType Intersection<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return std::min(mImpl->f1->eval(P), mImpl->f2->eval(P));
}
template <typename T>
const typename Volume<T>::volumeGradType Intersection<T>::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}


template class Intersection<float>;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Cutout<T>::Impl {
	public:
		boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f1,f2;
		float A;
	};
template <typename T>
Cutout<T>::Cutout(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) : mImpl(new Cutout<T>::Impl) {
	mImpl->f1 = _f1;
	mImpl->f2 = _f2;
}
	
template <typename T>
boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Cutout<T>::Ptr(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2) {
	return boost::shared_ptr<Volume<T> >(new Cutout<T>(_f1, _f2));
}

template <typename T>
const typename Volume<T>::volumeDataType Cutout<T>::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return std::min(mImpl->f1->eval(P), -mImpl->f2->eval(P));
}
template <typename T>
const typename Volume<T>::volumeGradType Cutout<T>::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}


template class Cutout<float>;



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
class AdvectVolume<T>::Impl {
public:
boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > f;
VolumeVectorPtr U;
float dt;
};
template<typename T>
AdvectVolume<T>::AdvectVolume(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f
        ,const VolumeVectorPtr _U
        , const float _dt) :
	mImpl(new AdvectVolume::Impl()) {
		mImpl->f = _f;
		mImpl->U = _U;
		mImpl->dt = _dt;

	}
template<typename T>
boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > AdvectVolume<T>::Ptr( const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1
        ,const VolumeVectorPtr _v2
        , const float dt) {
	return boost::shared_ptr<Volume<T> >(new AdvectVolume<T>(_f1, _v2, dt));
}
template<typename T>
const typename Volume<T>::volumeDataType AdvectVolume<T>::eval(const MPUtils::MPVec3 &P) const {
	return mImpl->f->eval(P - (mImpl->U->eval(P)) * mImpl->dt);
}
template<typename T>
const typename Volume<T>::volumeGradType AdvectVolume<T>::grad(const MPUtils::MPVec3 &P) const {}

template class AdvectVolume<float>;
template class AdvectVolume<MPUtils::MPVec3>;

class VectorNoise::Impl {
public:
boost::shared_ptr<MPNoise::FractalSum<MPNoise::PerlinNoiseGustavson> >perlin;
MPNoise::Noise_t parms;
boost::shared_ptr<MPNoise::Noise> noise;
};
VectorNoise::VectorNoise(const MPNoise::Noise_t _parms):
	mImpl(new VectorNoise::Impl()) {
		mImpl->parms = _parms;
		mImpl->perlin = MPNoise::FractalSum<MPNoise::PerlinNoiseGustavson>::Ptr();
		mImpl->perlin->setParameters(mImpl->parms);
		
		mImpl->noise = mImpl->perlin;
}
boost::shared_ptr<MeshPotato::MPVolume::Volume<MPUtils::MPVec3> > VectorNoise::Ptr( const MPNoise::Noise_t _parms) {
        return boost::shared_ptr<Volume<MPUtils::MPVec3> >(new VectorNoise(_parms));
}

const MPUtils::MPVec3 VectorNoise::eval(const MPUtils::MPVec3 &P) const {
	return MPUtils::MPVec3(mImpl->noise->eval(P)
		,mImpl->noise->eval(P + mImpl->parms.offset)
		,mImpl->noise->eval(P - mImpl->parms.offset));

}
const MPUtils::MPMat3 VectorNoise::grad(const MPUtils::MPVec3 &P) const {}

Identity::Identity() {}
boost::shared_ptr<MeshPotato::MPVolume::Volume<MPUtils::MPVec3> > Identity::Ptr() {
        return boost::shared_ptr<Volume<MPUtils::MPVec3> >(new Identity());
}

const MPUtils::MPVec3 Identity::eval(const MPUtils::MPVec3 &P) const {
        return P;
}
const MPUtils::MPMat3 Identity::grad(const MPUtils::MPVec3 &P) const {}

}
}
