/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Class for generating procedural FFT Noise
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
#ifndef __FFTNOISE_H__
#define __FFTNOISE_H__

#include "MeshPotato/MPUtils/Vector.h"
#include "MeshPotato/MPVolume/MPVolume.h"
#include "MeshPotato/MPVolume/MPVolumeGrid.h"
#include "Noise.h"

namespace MeshPotato {
	
	namespace MPNoise {
		/*!
		* \class FFTNoise
		* \brief Class for generating procedural FFT noise
		*/
		class FFTNoise : public Noise
		{
			public:

				FFTNoise(){}
				~FFTNoise(){}

				const float eval( const MeshPotato::MPUtils::MPVec3& P ) const { return grid.eval(P); }


				void setParameters( const Noise_t& parameters );
				void getParameters( Noise_t& parameters ) const {}


			private:

				MeshPotato::MPVolume::VolumeGrid<float> grid;

		};


	}
}
#endif
