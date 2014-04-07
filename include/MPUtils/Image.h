
#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <vector>
#include "Color.h"
#include <boost/shared_ptr.hpp>
using namespace std;

namespace MeshPotato {
namespace MPUtils {

class Image
{
  public:
    static boost::shared_ptr<Image> Ptr();
    Image();
   ~Image();

    void reset(  int w, int h, int d=4 );
    
    const float& value( int x, int y, int c ) const; 
    float& value( int x, int y, int c );

    std::vector<float>& pixel(int x, int y );
    std::vector<float>& pixel(int x );
   
    //! Interpolates a channel
    const float interpolatedValue( float x, float y, int c ) const; 
    //! Interpolates a pixel 
    std::vector<float> interpolatedPixel( float x, float y ) const;

    const int Width() const;
    const int Height() const;
    const int Depth() const; 
    Image(const Image&);

  private:
    class Impl;
    boost::shared_ptr<Impl> mImpl;
};

void setPixel( Image& img, int x, int y, const std::vector<float>& value );
void setPixel( Image& img, int &x, int &y, const Color &value );

}
}
#endif



