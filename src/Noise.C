
#include "MPNoise/Noise.h"
namespace MeshPotato {
	namespace MPNoise {



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
