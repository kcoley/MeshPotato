/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Collection of Field Algebra classes for performing operations on volumes
///
///   Copyright 2014 Kacey Coley
///   Licensed under the Apache License, Version 2.0 (the "License");
///   you may not use this file except in compliance with the License.
///   You may obtain a copy of the License at
///
///   http://www.apache.org/licenses/LICENSE-2.0
///
///   Unless required by applicable law or agreed to in writing, software
///   distributed under the License is distributed on an "AS IS" BASIS,
///   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
///   See the License for the specific language governing permissions and
///   limitations under the License.
///

#ifndef __FIELD_ALGEBRA_H__
#define __FIELD_ALGEBRA_H__
#include "MPVolume.h"
#include <boost/shared_ptr.hpp>
#include "MeshPotato/MPNoise/Noise.h"
#include "MeshPotato/MPNoise/PerlinNoise.h"
namespace MeshPotato {
namespace MPVolume {
	/// Add two resolution independent volumes together
template <typename T>
class AddVolume: public MeshPotato::MPVolume::Volume<T> {
public:
	AddVolume(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1,
	  		  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2);
	
	static boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Ptr(
	  		  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1,
	  		  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2);

/// returns the sum of the two volumes at a point in world space
const typename Volume<T>::volumeDataType  eval(const MeshPotato::MPUtils::MPVec3 &P) const;
/// computes the gradient of the sum of the two volumes (not implemented yet)
const typename Volume<T>::volumeGradType grad(
	  const MeshPotato::MPUtils::MPVec3 &P) const;
private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};
/// Advect a resolution independent volume with a velocity field
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
	/// Returns the advected value at a point in world space
const typename Volume<T>::volumeDataType eval(const MPUtils::MPVec3 &P) const;
/// Returns the gradient at a point in world space
const typename Volume<T>::volumeGradType grad(const MPUtils::MPVec3 &P) const;
private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};
typedef AdvectVolume<float> AdvectVolumeFloat;
typedef AdvectVolume<MPUtils::MPVec3> AdvectVolumeVector;

/// Create a vector noise velocity field (based on Perlin Noise)
class VectorNoise:public Volume <MPUtils::MPVec3> {
public:
VectorNoise(const MPNoise::Noise_t  _parms);
static boost::shared_ptr<MeshPotato::MPVolume::Volume<MPUtils::MPVec3> > Ptr(
	const MPNoise::Noise_t);
	/// Returns a vector at a point in world space
const MPUtils::MPVec3  eval(const MPUtils::MPVec3 &P) const;
/// Returns a matrix at a point in world space (not implemented yet)
const MPUtils::MPMat3 grad(const MPUtils::MPVec3 &P) const;
public:
class Impl;
boost::shared_ptr<Impl> mImpl;
};

/// Return the value passed in
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

/// Union two resolution independent volumes together
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

/// Intersect two resolution independent volumes together
template <typename T>
class Intersection: public MeshPotato::MPVolume::Volume<T> {
public:
Intersection(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1,
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

typedef Intersection<float> IntersectionFloat;

/// Blend two resolution independent volumes together
template <typename T>
class Blend: public MeshPotato::MPVolume::Volume<T> {
public:
Blend(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1,
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2,
	const float _A);

static boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > Ptr(
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1,
	  const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f2,
	  const float _A
	);

const typename Volume<T>::volumeDataType  eval(const MeshPotato::MPUtils::MPVec3 &P) const;
const typename Volume<T>::volumeGradType grad(
	  const MeshPotato::MPUtils::MPVec3 &P) const;
private:
class Impl;
boost::shared_ptr<Impl> mImpl;
};



typedef Blend<float> BlendFloat;

/// Cutout one resolution independent volume from another
template <typename T>
class Cutout: public MeshPotato::MPVolume::Volume<T> {
public:
Cutout(const boost::shared_ptr<MeshPotato::MPVolume::Volume<T> > _f1,
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
typedef Cutout<float> CutoutFloat;

}
}
#endif // __FIELD_ALGEBRA_H__
