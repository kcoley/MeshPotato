#include <fftw3.h>
#include "MPVolumeGrid.h"
#include "DenseGrid.h"
namespace MeshPotato {
	namespace MPVolume {

VolumeVectorPtr FFTDivFree( VolumeGridVectorPtr divfreeU, VolumeVectorPtr U )
{
	fftw_complex *datax, *datay, *dataz;
	fftw_plan pforwardx, pbackwardx;
	fftw_plan pforwardy, pbackwardy;
	fftw_plan pforwardz, pbackwardz;
	datax = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * divfreeU->nx()*divfreeU->ny()*divfreeU->nz());
	datay = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * divfreeU->nx()*divfreeU->ny()*divfreeU->nz());
	dataz = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * divfreeU->nx()*divfreeU->ny()*divfreeU->nz());
	pforwardx = fftw_plan_dft_3d( divfreeU->nx(), divfreeU->ny(), divfreeU->nz(), datax, datax, FFTW_FORWARD, FFTW_ESTIMATE);
	pbackwardx = fftw_plan_dft_3d( divfreeU->nx(), divfreeU->ny(), divfreeU->nz(), datax, datax, FFTW_BACKWARD, FFTW_ESTIMATE);
	pforwardy = fftw_plan_dft_3d( divfreeU->nx(), divfreeU->ny(), divfreeU->nz(), datay, datay, FFTW_FORWARD, FFTW_ESTIMATE);
	pbackwardy = fftw_plan_dft_3d( divfreeU->nx(), divfreeU->ny(), divfreeU->nz(), datay, datay, FFTW_BACKWARD, FFTW_ESTIMATE);
	pforwardz = fftw_plan_dft_3d( divfreeU->nx(), divfreeU->ny(), divfreeU->nz(), dataz, dataz, FFTW_FORWARD, FFTW_ESTIMATE);
	pbackwardz = fftw_plan_dft_3d( divfreeU->nx(), divfreeU->ny(), divfreeU->nz(), dataz, dataz, FFTW_BACKWARD, FFTW_ESTIMATE);


	divfreeU->stamp(U);
	
	long index = 0;
	for( int k=0;k<divfreeU->nz();k++ )
	{
		for( int j=0;j<divfreeU->ny();j++ )
		{
			for( int i=0;i<divfreeU->nx();i++ )
			{
				MPUtils::Coord ijk(i,j,k);
				MPUtils::MPVec3 value = divfreeU->get(ijk);
				datax[index][0] = value.x();
				datax[index][1] = 0;
				datay[index][0] = value.y();
				datay[index][1] = 0;
				dataz[index][0] = value.z();
				dataz[index][1] = 0;
				++index;
			}
		}
	}


	fftw_execute(pforwardx);
	fftw_execute(pforwardy);
	fftw_execute(pforwardz);


	float dKx = 2.0 * M_PI  / divfreeU->nx();
	float dKy = 2.0 * M_PI  / divfreeU->ny();
	float dKz = 2.0 * M_PI  / divfreeU->nz();
	float nyqK = 2.0 * M_PI;
	float scaler = 1.0/(divfreeU->nx() * divfreeU->ny() * divfreeU->nz() );
	index = 0;
	for( int k=0;k<divfreeU->nz();k++ )
	{
		float kz = k * dKz;
		if( k > divfreeU->nz()/2 ){ kz -= nyqK; }
		for( int j=0;j<divfreeU->ny();j++ )
		{
			float ky = j * dKy;
			if( j > divfreeU->ny()/2 ){ ky -= nyqK; }
			for( int i=0;i<divfreeU->nx();i++ )
			{
				float kx = i * dKx;
				if( i > divfreeU->nx()/2 ){ kx -= nyqK; }
				float ksq = kx*kx + ky*ky + kz*kz;

				if( ksq == 0.0 )
				{
					datax[index][0] = datax[index][1] = 0.0;
					datay[index][0] = datay[index][1] = 0.0;
					dataz[index][0] = dataz[index][1] = 0.0;
				}
				else
				{
					float vdotkr = kx*datax[index][0] + ky*datay[index][0] + kz*dataz[index][0];
					float vdotki = kx*datax[index][1] + ky*datay[index][1] + kz*dataz[index][1];

					datax[index][0] -= vdotkr * kx / ksq;
					datay[index][0] -= vdotkr * ky / ksq;
					dataz[index][0] -= vdotkr * kz / ksq;

					datax[index][1] -= vdotki * kx / ksq;
					datay[index][1] -= vdotki * ky / ksq;
					dataz[index][1] -= vdotki * kz / ksq;

					datax[index][0] *= scaler;
					datay[index][0] *= scaler;
					dataz[index][0] *= scaler;

					datax[index][1] *= scaler;
					datay[index][1] *= scaler;
					dataz[index][1] *= scaler;
				}
				++index;
			}
		}
	}


	fftw_execute(pbackwardx);
	fftw_execute(pbackwardy);
	fftw_execute(pbackwardz);

	VolumeGridVectorPtr divfreeU2 = divfreeU->deepCopy();
	index = 0;
	for( int k=0;k<divfreeU->nz();k++ )
	{
		for( int j=0;j<divfreeU->ny();j++ )
		{
			for( int i=0;i<divfreeU->nx();i++ )
			{
				MPUtils::Coord ijk(i,j,k);
				divfreeU2->set(ijk, MPUtils::MPVec3( datax[index][0], datay[index][0], dataz[index][0] ));
				++index;
			}
		}
	}


	fftw_destroy_plan(pforwardx);
	fftw_destroy_plan(pbackwardx);
	fftw_free(datax);

	fftw_destroy_plan(pforwardy);
	fftw_destroy_plan(pbackwardy);
	fftw_free(datay);

	fftw_destroy_plan(pforwardz);
	fftw_destroy_plan(pbackwardz);
	fftw_free(dataz);

	return divfreeU2;
}

}
}
