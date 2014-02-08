
#ifndef __DEEPIMAGE_H__
#define __DEEPIMAGE_H__

#include <vector>
#include <list>
#include "Color.h"
using namespace std;

namespace MeshPotato {
	namespace MPUtils {

		struct DeepPixel{
			Color  color;
			float depth;
		};
		typedef std::list<DeepPixel> DeepPixelBuffer;
		typedef std::vector<DeepPixel> DeepPixelBufferVector;
		class DeepImage
		{
			public:

				DeepImage() :
					width  (0),
					height (0),
					depth  (0)
			{}

				~DeepImage(){}

				void reset(  int w, int h, int d=5 )
				{
					width = w;
					height = h;
					depth = d;
					data.resize(width*height);
				}

				const DeepPixelBuffer& value( int x, int y) const { return data[ index(x,y) ]; }
				const DeepPixelBufferVector getDeepPixelBufferVector( int x, int y) { 
					DeepPixelBuffer deeplist = data[index(x,y)];
					DeepPixelBufferVector pixels(deeplist.begin(), deeplist.end());
					return pixels; 
				}

				DeepPixelBuffer& value(int x, int y ) { return data[ index(x,y) ]; }
				DeepPixelBuffer& pixel(int x ) { return data[x]; }

				const int Width() const { return width; }
				const int Height() const { return height; }
				const int Depth() const { return depth; }

			private:

				int width, height, depth;
				std::vector< DeepPixelBuffer > data;

				const size_t index( int x, int y ) const { return (size_t) ( x + width*y ) ; }
		};
		void setPixel( DeepImage& img, int x, int y, DeepPixelBuffer value );
		void setPixel( DeepImage& img, int x, int y, DeepPixel &value );

	}
}
#endif // __DEEPIMAGE_H__



