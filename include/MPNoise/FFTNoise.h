
#ifndef __FFTNOISE_H__
#define __FFTNOISE_H__

#include "MPUtils/Vector.h"
#include "MPVolume/MPVolume.h"
#include "MPVolume/MPVolumeGrid.h"
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
