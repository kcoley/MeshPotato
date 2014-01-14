
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <vector>
#include "Color.h"
using namespace std;

namespace MeshPotato {
namespace MPUtils {

class Image
{
  public:

    Image() :
       width  (0),
       height (0),
       depth  (0)
    {}

   ~Image(){}

    void reset(  int w, int h, int d=4 )
    {
       width = w;
       height = h;
       depth = d;
       std::vector<float> pixel;
       pixel.resize(depth);
       for( size_t i=0;i<(size_t)depth;i++ ){ pixel[i] = 0.0; }
       data.resize(width*height);
       for( size_t i=0;i<data.size();i++ ){ data[i] = pixel; }
    }
    
    const float& value( int x, int y, int c ) const { return data[ index(x,y) ][(size_t)c]; }
          float& value( int x, int y, int c ) { return data[ index(x,y) ][(size_t)c]; }

    std::vector<float>& pixel(int x, int y ) { return data[ index(x,y) ]; }
    std::vector<float>& pixel(int x ) { return data[x]; }
   
    //! Interpolates a channel
    const float interpolatedValue( float x, float y, int c ) const; 
    //! Interpolates a pixel 
    std::vector<float> interpolatedPixel( float x, float y ) const;

    const int Width() const { return width; }
    const int Height() const { return height; }
    const int Depth() const { return depth; }

  private:

    int width, height, depth;
    std::vector< std::vector<float> > data;
    
    const size_t index( int x, int y ) const { return (size_t) ( x + width*y ) ; }

    void interpolationCoefficients( float x, float y, 
                                   float& wx, float& wwx,
				   float& wy, float& wwy,
				   int& ix, int& iix,
				   int& iy, int& iiy 
				 ) const;
};

void setPixel( Image& img, int x, int y, std::vector<float>& value );
void setPixel( Image& img, int x, int y, Color value );

}
}
#endif



