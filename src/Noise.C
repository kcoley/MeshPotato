#include "MPNoise/Noise.h"
namespace MeshPotato {
	namespace MPNoise {

		template <typename BaseNoise>
		class FractalSum<BaseNoise>::Impl {
		public:
			Impl() : 
					noise         (),
					octaves    (3.0),
					fjump      (2.0),
					roughness  (0.5),
					frequency  (0.666666),
					translate  (MeshPotato::MPUtils::MPVec3(0,0,0)),
					offset     (0.0),
					axis       (MeshPotato::MPUtils::MPVec3(0,0,1)),
					angle      (0.0)
					{}

				BaseNoise noise;

				float octaves;
				float fjump;
				float roughness;
				float frequency;
				MeshPotato::MPUtils::MPVec3 translate;
				float offset;
				MeshPotato::MPUtils::MPVec3 axis;
				float angle;
		};

		template <typename BaseNoise>
		boost::shared_ptr<FractalSum<BaseNoise> > FractalSum<BaseNoise>::Ptr() { return boost::shared_ptr<FractalSum<BaseNoise> >(new FractalSum<BaseNoise>()); }

		template <typename BaseNoise>
		FractalSum<BaseNoise>::FractalSum() : mImpl(new Impl()) {}

		template <typename BaseNoise>
		FractalSum<BaseNoise>::~FractalSum(){}

				template <typename BaseNoise>
				const float FractalSum<BaseNoise>::eval( const float x ) const
				{
					float exponent = 1;
					float amplitude = 1;
					float accum = 0;
					int ioct = (int)mImpl->octaves;
					for( int oc=0;oc<ioct;oc++ )
					{
						const float X = (x - mImpl->translate[0]) * mImpl->frequency * exponent;
						accum += amplitude * mImpl->noise.eval( X );
						exponent *= mImpl->fjump;
						amplitude *= mImpl->roughness;
					}
					const float X = (x - mImpl->translate[0]) * mImpl->frequency * exponent;
					float val = amplitude * mImpl->noise.eval( X );
					accum += (mImpl->octaves - (int)mImpl->octaves) * val;
					return (accum + mImpl->offset);
				}

				template <typename BaseNoise>
				const float FractalSum<BaseNoise>::eval( const MeshPotato::MPUtils::MPVec3& x ) const
				{
					float exponent = 1;
					float amplitude = 1;
					float accum = 0;
					int ioct = (int)mImpl->octaves;
					MeshPotato::MPUtils::MPVec3 X = (x - mImpl->translate);
					if( mImpl->angle != 0.0 )
					{
						float ca = cos(mImpl->angle);
						float sa = sin(mImpl->angle);
						X = X*ca + mImpl->axis*(mImpl->axis*X)*(1.0-ca) + (mImpl->axis.cross(X))*sa;
					}
					X *= mImpl->frequency*exponent;
					for( int oc=0;oc<ioct;oc++ )
					{
						accum += amplitude * mImpl->noise.eval( X );
						X *= mImpl->fjump;
						amplitude *= mImpl->roughness;
					}
					float val = amplitude * mImpl->noise.eval( X );
					accum += (mImpl->octaves - (int)mImpl->octaves) * val;
					return (accum+mImpl->offset);
				}

				template <typename BaseNoise>
				void FractalSum<BaseNoise>::setParameters( const Noise_t& parameters )
				{
					mImpl->octaves = parameters.octaves;
					mImpl->fjump = parameters.fjump;
					mImpl->roughness = parameters.roughness;
					mImpl->frequency = parameters.frequency;
					mImpl->translate = parameters.translate;
					mImpl->offset = parameters.offset;
					mImpl->axis = parameters.axis;
					mImpl->angle = parameters.angle;
					mImpl->noise.setTime( parameters.time );
				}

				template <typename BaseNoise>
				void FractalSum<BaseNoise>::getParameters( Noise_t& parameters ) const
				{
					parameters.octaves = mImpl->octaves;
					parameters.fjump = mImpl->fjump;
					parameters.roughness = mImpl->roughness;
					parameters.frequency = mImpl->frequency;
					parameters.translate = mImpl->translate;
					parameters.offset = mImpl->offset;
					parameters.axis = mImpl->axis;
					parameters.angle = mImpl->angle;
				}



		const Anchor evaluateAnchorChain( const AnchorChain& anchorList, const double x )
		{
			int anchorsize = (int)anchorList.size();
			int anchor = x * anchorsize;
			double w = x * anchorsize - (double)anchor;
			if( anchor < 0 )
			{ 
				anchor = 0;
				w = 0;
			}
			if( anchor >= anchorsize-1 )
			{
				anchor = anchorsize-2;
				w = 1;
			}

			double w0 = 1.0 - w;
			Anchor value;
			value.frequency                 = anchorList[anchor].frequency * w0                 + anchorList[anchor+1].frequency * w;
			value.translate                 = anchorList[anchor].translate * w0                 + anchorList[anchor+1].translate * w; 
			value.octaves                   = anchorList[anchor].octaves * w0                   + anchorList[anchor+1].octaves * w;
			value.amplitude                 = anchorList[anchor].amplitude * w0                 + anchorList[anchor+1].amplitude * w;
			value.offset                    = anchorList[anchor].offset * w0                    + anchorList[anchor+1].offset * w;
			value.fjump                     = anchorList[anchor].fjump * w0                     + anchorList[anchor+1].fjump * w;
			value.roughness                 = anchorList[anchor].roughness * w0                 + anchorList[anchor+1].roughness * w;
			value.radius                    = anchorList[anchor].radius * w0                    + anchorList[anchor+1].radius * w;
			value.pscale                    = anchorList[anchor].pscale * w0                    + anchorList[anchor+1].pscale * w;
			value.amplitude                 = anchorList[anchor].amplitude * w0                 + anchorList[anchor+1].amplitude * w;
			value.gamma                     = anchorList[anchor].gamma * w0                     + anchorList[anchor+1].gamma * w;
			value.time                      = anchorList[anchor].time * w0                      + anchorList[anchor+1].time * w;
			value.fftLowCutoff              = anchorList[anchor].fftLowCutoff * w0              + anchorList[anchor+1].fftLowCutoff * w;
			value.fftHighCutoff             = anchorList[anchor].fftHighCutoff * w0             + anchorList[anchor+1].fftHighCutoff * w;
			value.fftPower                  = anchorList[anchor].fftPower * w0                  + anchorList[anchor+1].fftPower * w;
			value.fftNbGridPoints           = anchorList[anchor].fftNbGridPoints * w0           + anchorList[anchor+1].fftNbGridPoints * w;
			value.fftLength                 = anchorList[anchor].fftLength * w0                 + anchorList[anchor+1].fftLength * w;
			value.lognormalmean             = anchorList[anchor].lognormalmean * w0             + anchorList[anchor+1].lognormalmean * w;
			value.gaussianstandarddeviation = anchorList[anchor].gaussianstandarddeviation * w0 + anchorList[anchor+1].gaussianstandarddeviation * w;
			value.seed                      = anchorList[anchor].seed * w0                      + anchorList[anchor+1].seed * w;
			value.tangent                   = anchorList[anchor].tangent * w0                   + anchorList[anchor+1].tangent * w;
			value.normal                    = anchorList[anchor].normal * w0                    + anchorList[anchor+1].normal * w;
			value.binormal                  = anchorList[anchor].binormal * w0                  + anchorList[anchor+1].binormal * w;
			value.axis                      = anchorList[anchor].axis * w0                      + anchorList[anchor+1].axis * w;
			value.angle                     = anchorList[anchor].angle * w0                     + anchorList[anchor+1].angle * w;
			value.P                         = anchorList[anchor].P * w0                         + anchorList[anchor+1].P * w;
			value.v                         = anchorList[anchor].v * w0                         + anchorList[anchor+1].v * w;
			value.A                         = anchorList[anchor].A * w0                         + anchorList[anchor+1].A * w;

			return value;
		}

	}
}
