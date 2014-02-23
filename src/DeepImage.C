#include "MPUtils/DeepImage.h"
#include <iostream>
using namespace std;
namespace MeshPotato {
	namespace MPUtils {

		void addPixel( DeepImage& img, int x, int y, const DeepPixelBuffer &value )
		{
			for (DeepPixelBuffer::const_iterator iter = value.begin(); iter != value.end(); ++iter) 
				img.value( x, y).push_back((*iter));
		}

		void addPixel( DeepImage& img, int x, int y, const DeepPixel &value )
		{
			img.value(x,y).push_back(value);
		}

		void setPixel( DeepImage& img, int x, int y, const DeepPixelBuffer &value )
		{
			img.value(x,y).clear();
			addPixel(img, x,y,value);
		}

		void setPixel( DeepImage& img, int x, int y, const DeepPixel &value )
		{
			img.value(x,y).clear();
			addPixel(img, x,y,value);
		}
	}
}
