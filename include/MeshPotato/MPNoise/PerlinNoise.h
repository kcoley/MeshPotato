/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Class for generating Perlin Noise
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

#ifndef __PERLINNOISE_H__
#define __PERLINNOISE_H__

#include "MeshPotato/MPUtils/Vector.h"
#include <boost/shared_ptr.hpp>
namespace MeshPotato {


	namespace MPNoise
	{
		/// Perlin Noise Class
		class PerlinNoise
		{
			public:
				boost::shared_ptr<PerlinNoise> Ptr();
				PerlinNoise();
				~PerlinNoise();

				void setTime( const float time );

				const float eval( float arg ) const;
				const float eval( const MeshPotato::MPUtils::MPVec3& vec ) const;

			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;

		};


		// This is Stefan Gustavson's DSOnoise implementation, ported to a C++ class.
		class PerlinNoiseGustavson
		{
			public:
				boost::shared_ptr<PerlinNoiseGustavson> Ptr();

				PerlinNoiseGustavson();

				~PerlinNoiseGustavson();


				void setTime( const float t );

				const float eval( float arg ) const;


				const float eval( const MeshPotato::MPUtils::MPVec3& vec ) const;

			private:
				class Impl;

				boost::shared_ptr<Impl> mImpl;

		};






	}
}
#endif
