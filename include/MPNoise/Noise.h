

//
//
// noise.h
//
// General purpose noise code
//
// Integrates multiple noise sources into a single framework
//
//
//


#ifndef __NOISE_H__
#define __NOISE_H__
#include "MPUtils/Vector.h"
#include "MPUtils/Color.h"
#include "MPUtils/OfficialColors.h"
#include <boost/shared_ptr.hpp>
#include <vector>
namespace MeshPotato {
	namespace MPNoise {
		/*! \struct Noise_t
		*  \brief A set of noise parameters
		*/
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

			float frequency;
			MeshPotato::MPUtils::MPVec3 translate;
			float octaves;
			float amplitude;
			float offset;
			float fjump;
			float roughness;
			float radius;
			float pscale;
			float gamma;
			float time;
			float fftLowCutoff;
			float fftHighCutoff;
			float fftPower;
			int   fftNbGridPoints;
			float fftLength;
			float lognormalmean;
			float gaussianstandarddeviation;
			int   seed;
			MeshPotato::MPUtils::MPVec3 tangent;
			MeshPotato::MPUtils::MPVec3 normal;
			MeshPotato::MPUtils::MPVec3 binormal;
			MeshPotato::MPUtils::MPVec3 axis;
			float angle;
			MeshPotato::MPUtils::MPVec3 P;
			MeshPotato::MPUtils::MPVec3 v;
			MeshPotato::MPUtils::MPVec3 A;
			float  age;
			float lifeTime;
			float shutter;
			float frameRate;
			float falloff;
		};

		struct Wisp_t
		{

			Wisp_t() :
				clump			(1.0/3.0),
				scale			(1.0),
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

				Noise(){}
				virtual ~Noise(){}

				virtual const float eval( const float x ) const { return 0; }
				virtual const float eval( const MeshPotato::MPUtils::MPVec3& x ) const { return 0; }

				virtual void setParameters( const Noise_t& parameters ){}
				virtual void getParameters( Noise_t& parameters ) const {}
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

				PRN(){}
				virtual ~PRN(){}

				virtual const float eval(){ return 0; }

				virtual void setParameters( const Noise_t& parameters ){}
		};






	}
}
#endif
