/// \author Kacey Coley
/// \date December 11, 2014
/// \since 0.1
///
/// Class for generating Deep Images
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

#ifndef __DEEPIMAGE_H__
#define __DEEPIMAGE_H__

#include <vector>
#include <list>
#include "Color.h"
#include<boost/shared_ptr.hpp>
using namespace std;

namespace MeshPotato {
	namespace MPUtils {
		class Image;
		struct DeepPixel{
			Color  color;
			float depth_front;
			float depth_back;
		};
		typedef std::list<DeepPixel> DeepPixelBuffer;
		typedef std::vector<DeepPixel> DeepPixelBufferVector;
		class DeepImage
		{
			public:
				static boost::shared_ptr<DeepImage> Ptr();
				DeepImage();

				~DeepImage();

				void reset(  int w, int h, int d=5 );

				const DeepPixelBuffer& value( int x, int y) const;
				const DeepPixelBufferVector getDeepPixelBufferVector( int x, int y);

				DeepPixelBuffer& value(int x, int y );
				DeepPixelBuffer& pixel(int x );

				boost::shared_ptr<Image> flatten();
				const int Width() const;
				const int Height() const;
				const int Depth() const;

			private:
				class Impl;
				boost::shared_ptr<Impl> mImpl;

				
		};
		void setPixel( DeepImage& img, int x, int y, const DeepPixelBuffer &value );
		void setPixel( DeepImage& img, int x, int y, const DeepPixel &value );
		void addPixel( DeepImage& img, int x, int y, const DeepPixelBuffer &value );
		void addPixel( DeepImage& img, int x, int y, const DeepPixel &value );

	}
}
#endif // __DEEPIMAGE_H__



