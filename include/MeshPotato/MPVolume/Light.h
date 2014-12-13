/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Classes for working with Lights
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

#include "FrustumGrid.h"
#include <boost/shared_ptr.hpp>
#include "DenseGrid.h"
namespace MeshPotato {
	namespace MPVolume {
		/// Base class for lights

	class Light : public Volume<MPUtils::Color> {
	public:
		virtual const volumeDataType eval(const MPUtils::MPVec3 &P) const;
		virtual const volumeGradType grad(const MPUtils::MPVec3 &P) const;
		virtual ~Light();
	};


	class FrustumLight : public Light {
	public:
	static VolumeColorPtr Ptr(
		boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> _frustum,
		MeshPotato::MPUtils::Color _color);

		FrustumLight(boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> _frustum,
		MeshPotato::MPUtils::Color _color);

		virtual const FrustumLight::volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
		virtual const FrustumLight::volumeGradType grad (const MeshPotato::MPUtils::MPVec3 &P) const;
		private:
		class Impl;
		boost::shared_ptr<Impl> mImpl;
	};

        class DenseLight : public Light {
        public:
        static VolumeColorPtr Ptr(
               boost::shared_ptr<MeshPotato::MPVolume::DenseGrid<float> > _grid,
               MeshPotato::MPUtils::Color _color);

               DenseLight(boost::shared_ptr<MeshPotato::MPVolume::DenseGrid<float> > _grid,
               MeshPotato::MPUtils::Color _color);

               virtual const DenseLight::volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
               virtual const DenseLight::volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const;
        private:
        class Impl;
        boost::shared_ptr<Impl> mImpl;
        };
	}
}
