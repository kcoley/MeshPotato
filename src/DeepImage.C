#include "MPUtils/DeepImage.h"
#include <iostream>
using namespace std;
namespace MeshPotato {
	namespace MPUtils {

		void setPixel( DeepImage& img, int x, int y, DeepPixelBuffer value )
		{
			for (DeepPixelBuffer::iterator iter = value.begin(); iter != value.end(); ++iter) 
				img.value( x, y).push_back((*iter));
		}

		void setPixel( DeepImage& img, int x, int y, DeepPixel value )
		{
			img.value(x,y).push_back(value);
		}
	}
}
