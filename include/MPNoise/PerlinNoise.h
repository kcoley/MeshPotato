

#ifndef __PERLINNOISE_H__
#define __PERLINNOISE_H__

#include "MPUtils/Vector.h"
#include <boost/shared_ptr.hpp>
namespace MeshPotato {


	namespace MPNoise
	{
		/*! \class PerlinNoise
		* \brief Class for generating Perlin noise
		*/

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
