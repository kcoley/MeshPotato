
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



