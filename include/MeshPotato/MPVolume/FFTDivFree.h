/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
///  Uses FFT to make a velocity field divergent free
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

#include "MPVolumeGrid.h"
#include "DenseGrid.h"
namespace MeshPotato {
	namespace MPVolume {
		/// FFTDivFree makes a velocity field divergent free
		VolumeVectorPtr FFTDivFree( VolumeGridVectorPtr divfreeU, VolumeVectorPtr U );

	}
}
