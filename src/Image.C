#include "MPUtils/Image.h"
#include <iostream>
using namespace std;
namespace MeshPotato {
namespace MPUtils {

const float Image::interpolatedValue( float x, float y, int c ) const
{
   int ix, iy, iix, iiy;
   float wx, wy, wwx, wwy;
   interpolationCoefficients( x, y, wx, wwx, wy, wwy, ix, iix, iy, iiy );
   float v = value(  ix,  iy, c ) *  wx *  wy
           + value( iix,  iy, c ) * wwx *  wy
	   + value(  ix, iiy, c ) *  wx * wwy
	   + value( iix, iiy, c ) * wwx * wwy;
   return v;
}


std::vector<float> Image::interpolatedPixel( float x, float y ) const
{
   int ix, iy, iix, iiy;
   float wx, wy, wwx, wwy;
   interpolationCoefficients( x, y, wx, wwx, wy, wwy, ix, iix, iy, iiy );
   std::vector<float> pix;
   for( size_t c=0;c<Depth();c++ )
   {
      float v = value(  ix,  iy, c ) *  wx *  wy
              + value( iix,  iy, c ) * wwx *  wy
	      + value(  ix, iiy, c ) *  wx * wwy
	      + value( iix, iiy, c ) * wwx * wwy;
      pix.push_back( v );
   }
   return pix;
}

void imageLinearInterpolation( float x, float dx, int nx, int& i, int& ii, float& w, float& ww, bool isperiodic )
{
    float r = x/dx;
    i  =  r;
    if( !isperiodic )
    {
    if( i >= 0 && i < nx )
    {
       ii = i + 1;
       w = r-i;
       ww = 1.0 - w;
       if(  ii >= nx )
       {
	  ii = nx-1;
       }
    }
    else
    {
       i = ii = 0;
       w = ww = 0;
    }
    }
    else
    {
       w = r-i;
       while( i < 0 ){ i += nx; }
       while( i >= nx ){ i -= nx; }

       ii = i+1;
       ww = 1.0 - w;
       if( ii >= nx ){ ii -= nx; }
    }
}




void Image::interpolationCoefficients( float x, float y, 
                                      float& wx, float& wwx,
				      float& wy, float& wwy,
				      int& ix, int& iix,
				      int& iy, int& iiy 
			  	    ) const
{
   imageLinearInterpolation( x, 1.0/Width(), Width(), ix, iix, wx, wwx, false );
   imageLinearInterpolation( y, 1.0/Height(), Height(), iy, iiy, wy, wwy, false );
}


void setPixel( Image& img, int x, int y, std::vector<float>& value )
{
   size_t nb = ( value.size() < img.Depth() ) ? value.size() : img.Depth();
   for( size_t i=0;i<nb;i++ )
   {
      img.value( x, y, i ) = value[i];
   }
}

void setPixel( Image& img, int x, int y, Color value )
{
      img.value( x, y, 0 ) = value.X();
      img.value( x, y, 1 ) = value.Y();
      img.value( x, y, 2 ) = value.Z();
      img.value( x, y, 3 ) = value.W();
}
}
}
