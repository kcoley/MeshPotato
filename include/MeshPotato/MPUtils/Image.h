/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Image class for writing images
///
///   Copyright 2014 Kacey Coley
///   Licensed under the Apache License, Version 2.0 (the "License");
///   you may not use this file except in compliance with the License.
///   You may obtain a copy of the License at
///
///   http://www.apache.org/licenses/LICENSE-2.0
///
///   Unless required by applicable law or agreed to in writing, software
///   distributed under the License is distributed on an "AS IS" BASIS,
///   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
///   See the License for the specific language governing permissions and
///   limitations under the License.
///

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



