/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// General Purpose Noise Generation Code
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

#ifndef __NOISE_H__
#define __NOISE_H__
#include "MPUtils/Vector.h"
#include "MPUtils/Color.h"
#include "MPUtils/OfficialColors.h"
#include <boost/shared_ptr.hpp>
#include <vector>
namespace MeshPotato {
	/// Class for generating Procedural Noise
	namespace MPNoise {
		/*! \struct Noise_t
		*  \brief A set of noise parameters
		*/
		/// \struct Basic set of noise parameters
		struct Noise_t
		{
			
			Noise_t() :
				frequency    (1),
				translate    (MeshPotato::MPUtils::MPVec3(0,0,0)),
				octaves      (1.0),
				amplitude    (1),
				offset       (0.1),
				fjump        (1),
				roughness    (0.3),
				radius       (1.0),
				pscale       (1.0),
				gamma        (1.0),
				time         (0.0),
				fftLowCutoff (0.01),
				fftHighCutoff (1.0),
				fftPower      (3.5),
				fftNbGridPoints (128),
				fftLength     (10.0),
				lognormalmean (1.0),
				gaussianstandarddeviation (1.0),
				seed         (12345),
				tangent      (MeshPotato::MPUtils::MPVec3(0,0,1)),
				normal       (MeshPotato::MPUtils::MPVec3(0,1,0)),
				binormal     (MeshPotato::MPUtils::MPVec3(1,0,0)),
				axis         (MeshPotato::MPUtils::MPVec3(0,1,0)),
				angle        (0.0),
				P            (MeshPotato::MPUtils::MPVec3(0,0,0)),
				v            (MeshPotato::MPUtils::MPVec3(0,0,0)),
				A            (MeshPotato::MPUtils::MPVec3(0,0,0)),
				age          (0.0),
				lifeTime     (1.0),
				shutter      (0.5),
				frameRate    (1.0/24.0),
				falloff      (1.0)

			{}

			float frequency; 								/// represents the noise frequency
			MeshPotato::MPUtils::MPVec3 translate; 			/// translates the noise spatially
			float octaves;									/// sets the noise octaves
			float amplitude;								/// sets the noise amplitude
			float offset;									/// sets the noise offset
			float fjump;									/// sets the noise fjump
			float roughness;								/// sets the noise roughness
			float radius;									/// sets the radius of the noise dot
			float pscale;									/// sets the noise pscale
			float gamma;									/// sets the noise gamma value
			float time;										/// indicates the time sample for noise
			float fftLowCutoff;								/// represents the fft low cut off frequency
			float fftHighCutoff;							/// represents the fft height cut off frequency
			float fftPower;									/// represents the power of fft noise
			int   fftNbGridPoints;							/// indicates the number of fft grid points
			float fftLength;								/// represents the length of fft
			float lognormalmean;							/// value for representing the log normal mean
			float gaussianstandarddeviation;				/// gaussian standard deviation
			int   seed;										/// sets the seed for the noise
			MeshPotato::MPUtils::MPVec3 tangent;			/// sets the tangent value for noise dot
			MeshPotato::MPUtils::MPVec3 normal;				/// sets the normal for the noise dot
			MeshPotato::MPUtils::MPVec3 binormal;			/// sets the binormal for the noise dot
			MeshPotato::MPUtils::MPVec3 axis;				/// represents the axis for the noise dot
			float angle;									/// represents the angle for a noise dot
			MeshPotato::MPUtils::MPVec3 P;					/// position
			MeshPotato::MPUtils::MPVec3 v;					/// velocity
			MeshPotato::MPUtils::MPVec3 A;					/// acceleration
			float  age;										/// represents the current age of a noise parameter
			float lifeTime;									/// represents the lifetime of a noise parameter
			float shutter;									/// represents the shutter time
			float frameRate;								/// represents the frame rate
			float falloff;									/// represents the falloff value
		};

		/// Represents a Wisp Type
		struct Wisp_t
		{

			Wisp_t() :
				clump			(1.0/3.0),
				scale			(1.0), /// sets the scale
				e0		(MeshPotato::MPUtils::MPVec3(-1,0,0)),
				e1		(MeshPotato::MPUtils::MPVec3(0,0,1)),
				e2		(MeshPotato::MPUtils::MPVec3(0,1,0)),
				t		(MeshPotato::MPUtils::MPVec3(0,0,0)),
				s		(MeshPotato::MPUtils::MPVec3(1,1,1)),
				fspn0			(0.4),
				P0i0		(MeshPotato::MPUtils::MPVec3(0,0,0.0)),
				P0i1		(MeshPotato::MPUtils::MPVec3(0.0,1.0,0.0)),
				amp		(1.0),
				correlation    (1),
				velocity    (MeshPotato::MPUtils::MPVec3(0,0,0)),
				seed         (12345),
				nbWispPoints (1000),
				P0            (MeshPotato::MPUtils::MPVec3(0,0,0)),
				color    (BLUE_RIDGE)
			{}
			Noise_t NP0, NP1;
			float clump, scale;
			MeshPotato::MPUtils::MPVec3 e0,e1,e2,t, s;
			float fspn0;
			MeshPotato::MPUtils::MPVec3 P0i0, P0i1;
			float amp;
			float correlation;
			MeshPotato::MPUtils::MPVec3 velocity;
			int   seed;
			int nbWispPoints;
			MeshPotato::MPUtils::MPVec3 P0;
			MeshPotato::MPUtils::Color color;
		};
		class Noise
		{
			public:

				Noise();
				virtual ~Noise();

				virtual const float eval( const float x ) const;
				virtual const float eval( const MeshPotato::MPUtils::MPVec3& x ) const;

				virtual void setParameters( const Noise_t& parameters );
				virtual void getParameters( Noise_t& parameters ) const;
		};


		typedef Noise_t Anchor;
		typedef std::vector<Anchor> AnchorChain;


		const Anchor evaluateAnchorChain( const AnchorChain& alist, const double x );


		typedef Wisp_t WispAnchor;
		typedef std::vector<WispAnchor> WispAnchorChain;


		const WispAnchor evaluateWispAnchorChain( const WispAnchorChain& alist, int anchor, const double x );


		template< typename BaseNoise>
		class FractalSum : public Noise
		{
			public:
				static boost::shared_ptr<FractalSum<BaseNoise> > Ptr();

				FractalSum();

				~FractalSum();

				const float eval( const float x ) const;

				const float eval( const MeshPotato::MPUtils::MPVec3& x ) const;


				void setParameters( const Noise_t& parameters );


				void getParameters( Noise_t& parameters ) const;



			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;
		};



		class PRN
		{
			public:

				PRN();
				virtual ~PRN();

				virtual const float eval();

				virtual void setParameters( const Noise_t &parameters );
		};






	}
}
#endif
