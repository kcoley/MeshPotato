/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Abstract Base Class for volumes
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

#ifndef __MPVOLUME_H__
#define __MPVOLUME_H__
#include "MeshPotato/MPUtils/Vector.h"
#include "MeshPotato/MPUtils/Color.h"
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
			/// Base class for volumes
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
