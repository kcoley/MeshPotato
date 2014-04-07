#ifndef __FIELD_ALGEBRA_H__
#define __FIELD_ALGEBRA_H__
#include "MPVolume.h"
#include <boost/shared_ptr.hpp>
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
}
}
#endif // __FIELD_ALGEBRA_H__
