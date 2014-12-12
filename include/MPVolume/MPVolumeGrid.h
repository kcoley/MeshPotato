/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Abstract Base Class for Volume Grids
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

#ifndef __MPVOLUMEGRID_H__
#define __MPVOLUMEGRID_H__
#include "MPVolume.h"

namespace MeshPotato {
	namespace MPVolume {
		//! Abstract Base class for volume grids
	template<typename T>
	class VolumeGrid: public Volume<T> {
	public:
		virtual ~VolumeGrid() {}

		virtual const typename Volume<T>::volumeDataType eval(const MPUtils::MPVec3 &P) const = 0;
		virtual const typename Volume<T>::volumeGradType grad(const MeshPotato::MPUtils::MPVec3& P) const = 0;
		virtual void set(const MPUtils::Coord ijk, const T value) = 0;
		virtual const T get(const MPUtils::Coord ijk) const= 0;
		virtual void add(const MPUtils::Coord ijk, const T value) = 0;
		virtual void stamp(boost::shared_ptr<Volume<T> > volume) = 0;
		virtual boost::shared_ptr<VolumeGrid<T> > deepCopy() = 0;

		virtual const int nx() const = 0;
		virtual const int ny() const = 0;
		virtual const int nz() const = 0;
		virtual	const MPUtils::MPVec3 indexToWorld(const MPUtils::Coord ijk) const =0;
		virtual const MPUtils::MPVec3 worldToIndex(const MPUtils::MPVec3 P) const =0;

	};
	typedef boost::shared_ptr<MeshPotato::MPVolume::VolumeGrid<float> > VolumeGridFloatPtr;
    typedef boost::shared_ptr<MeshPotato::MPVolume::VolumeGrid<MPUtils::MPVec3> > VolumeGridVectorPtr;
    typedef VolumeGrid<float> VolumeGridFloat;
    typedef VolumeGrid<MPUtils::MPVec3> VolumeGridVector;
	}
}
#endif // __MPVOLUMEGRID_H__
