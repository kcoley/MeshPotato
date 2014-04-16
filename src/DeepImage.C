#include "MPUtils/DeepImage.h"
#include "MPUtils/Image.h"
#include "MPUtils/Color.h"
#include <iostream>
using namespace std;
namespace MeshPotato {
    namespace MPUtils {
	class Tidy{
	public:
	  bool operator()(const DeepPixel lhs, const DeepPixel rhs) const {
		return lhs.depth_front > rhs.depth_front;
	  }
	};
	boost::shared_ptr<Image> DeepImage::flatten() {
	    boost::shared_ptr<Image> image = Image::Ptr();
	    image->reset(width, height);
	    for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
		    Color color(0,0,0,0);
		    DeepPixelBuffer buf = data[index(x,y)];
			buf.sort(MeshPotato::MPUtils::Tidy());
			for (DeepPixelBuffer::iterator iter = buf.begin(); iter != buf.end(); ++iter) {
			    Color tempcolor = iter->color + (color*(1 - iter->color.W()));
			    tempcolor[3] = iter->color[3] + color[3]*(1 - iter->color[3]);
                	    color = tempcolor;
			}
			std::vector<float> &pixel = image->pixel(x,y);
			pixel[0] = color[0];
			pixel[1] = color[1];
			pixel[2] = color[2];
			pixel[3] = color[3];
		    }
		}
		return image;

	    }
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
