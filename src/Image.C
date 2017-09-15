#include "MeshPotato/MPUtils/Image.h"
#include <iostream>

using namespace std;
namespace MeshPotato {
namespace MPUtils {
void imageLinearInterpolation( float x, float dx, int nx, int& i, int& ii, float& w, float& ww, bool isperiodic );
	class Image::Impl {
		public:
			int width, height, depth;
			std::vector<std::vector<float> > data;
			const int Width() const { return width;}
			const int Height() const { return height;}
			const int Depth() const { return depth;}
			const size_t index(int x, int y) const { return (size_t) (x + width*y);}
			void interpolationCoefficients( float x, float y,
		                                   float& wx, float& wwx,
	                                           float& wy, float& wwy,
                                                   int& ix, int& iix,
                                                   int& iy, int& iiy
                                                   ) const;

	};
	boost::shared_ptr<Image> Image::Ptr() { return boost::shared_ptr<Image>(new Image()); }
	Image::Image():
       		mImpl(new Image::Impl())	
	{
		mImpl->width = 0; 
	        mImpl->height = 0;
	        mImpl->depth = 0;
	}
	Image::~Image() {}
	void Image::reset(int w, int h, int d) {
		mImpl->width = w;
		mImpl->height = h;
		mImpl->depth = d;
		std::vector<float> pixel;
		pixel.resize(mImpl->depth);
		for( size_t i=0;i<(size_t)mImpl->depth;i++ ){ pixel[i] = 0.0; }
		mImpl->data.resize(mImpl->width*mImpl->height);
	        for( size_t i=0;i<mImpl->data.size();i++ ){ mImpl->data[i] = pixel; }
	}
	Image::Image(const Image& other) : 
		mImpl(new Image::Impl()) {
			mImpl->width = other.mImpl->width;
			mImpl->height = other.mImpl->height;
			mImpl->depth = other.mImpl->depth;
			mImpl->data = other.mImpl->data;
	}


	const float& Image::value( int x, int y, int c ) const { return mImpl->data[ mImpl->index(x,y) ][(size_t)c]; }
        float& Image::value( int x, int y, int c ) { return mImpl->data[ mImpl->index(x,y) ][(size_t)c]; }

        std::vector<float>& Image::pixel(int x, int y ) { return mImpl->data[ mImpl->index(x,y) ]; }
        std::vector<float>& Image::pixel(int x ) { return mImpl->data[x]; }

	const int Image::Width() const { return mImpl->Width(); }
    	const int Image::Height() const { return mImpl->Height(); }
        const int Image::Depth() const { return mImpl->Depth(); }


const float Image::interpolatedValue( float x, float y, int c ) const
{
   int ix, iy, iix, iiy;
   float wx, wy, wwx, wwy;
   mImpl->interpolationCoefficients( x, y, wx, wwx, wy, wwy, ix, iix, iy, iiy );
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
   mImpl->interpolationCoefficients( x, y, wx, wwx, wy, wwy, ix, iix, iy, iiy );
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




void Image::Impl::interpolationCoefficients( float x, float y, 
                                      float& wx, float& wwx,
				      float& wy, float& wwy,
				      int& ix, int& iix,
				      int& iy, int& iiy 
			  	    ) const
{
   imageLinearInterpolation( x, 1.0/Width(), Width(), ix, iix, wx, wwx, false );
   imageLinearInterpolation( y, 1.0/Height(), Height(), iy, iiy, wy, wwy, false );
}


void setPixel( Image& img, int x, int y, const std::vector<float>& value ) 
{
   size_t nb = ( value.size() < img.Depth() ) ? value.size() : img.Depth();
   for( size_t i=0;i<nb;i++ )
   {
      img.value( x, y, i ) = value[i];
   }
}

void setPixel( Image& img, int x, int y, const Color &value )
{
      img.value( x, y, 0 ) = value.X();
      img.value( x, y, 1 ) = value.Y();
      img.value( x, y, 2 ) = value.Z();
      img.value( x, y, 3 ) = value.W();
}
}
}
