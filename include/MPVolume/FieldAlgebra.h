#ifndef __FIELD_ALGEBRA_H__
#define __FIELD_ALGEBRA_H__
#include "MPVolume.h"
#include <boost/shared_ptr.hpp>
#include "MPNoise/Noise.h"
#include "MPNoise/PerlinNoise.h"
namespace MeshPotato {
namespace MPVolume {
template <typename T>
class AddVolume: public MeshPotato::MPVolume::Volume<T> {
public:
AddVolume(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, 
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2);
static boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Ptr(
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, 
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2);

const typename Volume<T>::volumeDataType  eval(const MeshPotato::MPUtils::MPVec3 &P) const;
const typename Volume<T>::volumeGradType grad(
	  const MeshPotato::MPUtils::MPVec3 &P) const;
private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};

template <typename T>
class AdvectVolume:public Volume <T> {
public:
AdvectVolume(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1
	,const VolumeVectorPtr _v1
	, const float dt);
static boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Ptr(
	const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1
	,const VolumeVectorPtr _v2
	, const float dt);
const typename Volume<T>::volumeDataType eval(const MPUtils::MPVec3 &P) const;
const typename Volume<T>::volumeGradType grad(const MPUtils::MPVec3 &P) const;
private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};
typedef AdvectVolume<float> AdvectVolumeFloat;
typedef AdvectVolume<MPUtils::MPVec3> AdvectVolumeVector;


class VectorNoise:public Volume <MPUtils::MPVec3> {
public:
VectorNoise(const MPNoise::Noise_t  _parms);
static boost::shared_ptr<MeshPotato::MPVolume::Volume<MPUtils::MPVec3> > Ptr(
	const MPNoise::Noise_t);
const MPUtils::MPVec3  eval(const MPUtils::MPVec3 &P) const;
const MPUtils::MPMat3 grad(const MPUtils::MPVec3 &P) const;
public:
class Impl;
boost::shared_ptr<Impl> mImpl;
};


class Identity:public Volume <MPUtils::MPVec3> {
public:
Identity();
static boost::shared_ptr<MeshPotato::MPVolume::Volume<MPUtils::MPVec3> > Ptr();
const MPUtils::MPVec3  eval(const MPUtils::MPVec3 &P) const;
const MPUtils::MPMat3 grad(const MPUtils::MPVec3 &P) const;
};

typedef AddVolume<float> AddVolumeFloat;
typedef AddVolume<MPUtils::Color> AddVolumeColor;
typedef AddVolume<MPUtils::MPVec3> AddVolumeVector;

template <typename T>
class Union: public MeshPotato::MPVolume::Volume<T> {
public:
Union(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, 
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2);
static boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Ptr(
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1, 
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2);

const typename Volume<T>::volumeDataType  eval(const MeshPotato::MPUtils::MPVec3 &P) const;
const typename Volume<T>::volumeGradType grad(
	  const MeshPotato::MPUtils::MPVec3 &P) const;
private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};

typedef Union<float> UnionFloat;
typedef Union<MPUtils::MPVec3> UnionVector;

//class FFTDivFree: public Volume<MPUtils::MPVec3> {
//public:
//FFTDivFree(const boost::shared_ptr<MeshPotato::MPVolume::VolumeGrid<T> > grid);
//};
}
}
#endif // __FIELD_ALGEBRA_H__
