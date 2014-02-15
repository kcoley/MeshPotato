
#include <cstdlib>
#include "MPNoise/FFTNoise.h"
#include <fftw3.h>



namespace MeshPotato {
	namespace MPNoise {


		void FFTNoise::setParameters( const Noise_t& n )
		{
			fftw_complex *data;
			fftw_plan pforward, pbackward;
			data = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * n.fftNbGridPoints*n.fftNbGridPoints*n.fftNbGridPoints);
			pforward = fftw_plan_dft_3d( n.fftNbGridPoints, n.fftNbGridPoints, n.fftNbGridPoints, data, data, FFTW_FORWARD, FFTW_ESTIMATE);
			pbackward = fftw_plan_dft_3d( n.fftNbGridPoints, n.fftNbGridPoints, n.fftNbGridPoints, data, data, FFTW_BACKWARD, FFTW_ESTIMATE);

			srand48(n.seed);

			double mean = 0;
			for( long k=0;k<n.fftNbGridPoints*n.fftNbGridPoints*n.fftNbGridPoints;k++ )
			{
				data[k][0] = drand48()-0.5;
				data[k][1] = 0;
				mean += data[k][0];
			}

			mean /= pow( (float)n.fftNbGridPoints, (int)3 );

			for( long k=0;k<n.fftNbGridPoints*n.fftNbGridPoints*n.fftNbGridPoints;k++ )
			{
				data[k][0] -= mean;
			}

			fftw_execute(pforward);

			float dK = 2.0 * M_PI  / n.fftLength;
			float nyqK = 2.0 * M_PI * n.fftNbGridPoints/n.fftLength;
			long index = 0;
			for( int k=0;k<n.fftNbGridPoints;k++ )
			{
				float kz = k * dK;
				if( k > n.fftNbGridPoints/2 ){ kz -= nyqK; }
				for( int j=0;j<n.fftNbGridPoints;j++ )
				{
					float ky = j * dK;
					if( j > n.fftNbGridPoints/2 ){ ky -= nyqK; }
					for( int i=0;i<n.fftNbGridPoints;i++ )
					{
						float kx = i * dK;
						if( i > n.fftNbGridPoints/2 ){ kx -= nyqK; }
						float ksq = kx*kx + ky*ky + kz*kz;

						float amp = 0.0;
						if( ksq > 0 ) { amp = exp( -ksq*n.fftLowCutoff*n.fftLowCutoff ) * exp( - 1.0/(ksq*n.fftHighCutoff*n.fftHighCutoff) ) / pow( (double)ksq, (double)n.fftPower*0.5 ); }

						data[index][0] *= amp;
						data[index][1] *= amp;
						++index;
					}
				}
			}

			fftw_execute(pbackward);

			float scaler = 1.0/pow( (float)n.fftNbGridPoints,(int)3);

			grid.init( n.fftNbGridPoints, n.fftNbGridPoints, n.fftNbGridPoints, n.fftLength, n.fftLength, n.fftLength, MeshPotato::MPUtils::MPVec3(0,0,0) );
			grid.setPeriodic();
			index = 0;
			mean = 0;
			double stddev = 0;
			for( int k=0;k<n.fftNbGridPoints;k++ )
			{
				for( int j=0;j<n.fftNbGridPoints;j++ )
				{
					for( int i=0;i<n.fftNbGridPoints;i++ )
					{
						float val = data[index][0] *scaler;
						grid.value(i,j,k) = val;
						mean += val;
						stddev += val*val;
						++index;
					}
				}
			}

			fftw_destroy_plan(pforward);
			fftw_destroy_plan(pbackward);
			fftw_free(data);


			mean /= pow( (float)n.fftNbGridPoints, (int)3 );
			stddev /= pow( (float)n.fftNbGridPoints, (int)3 );
			stddev -= mean*mean;
			stddev = sqrt(stddev);
			for( int k=0;k<n.fftNbGridPoints;k++ )
			{
				for( int j=0;j<n.fftNbGridPoints;j++ )
				{
					for( int i=0;i<n.fftNbGridPoints;i++ )
					{
						grid.value(i,j,k)  -= mean;
						grid.value(i,j,k)  /= stddev;
					}
				}
			}
		}
	}
}
