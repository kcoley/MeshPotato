#include "MeshPotato/MPUtils/DeepImage.h"
#include "MeshPotato/MPUtils/Image.h"
#include "MeshPotato/MPUtils/Color.h"
#include <iostream>
using namespace std;
namespace MeshPotato {
    namespace MPUtils {
    	class DeepImage::Impl {
    	public:
    		Impl() :
					width  (0),
					height (0),
					depth  (0) {}

    		int width, height, depth;
			std::vector< DeepPixelBuffer > data;

			const size_t index( int x, int y ) const { return (size_t) ( x + width*y ) ; }
    	};

	class Tidy{
	public:
	  bool operator()(const DeepPixel lhs, const DeepPixel rhs) const {
		return lhs.depth_front > rhs.depth_front;
	  }
	};

	DeepImage::DeepImage() : mImpl(new Impl()) {}

	DeepImage::~DeepImage(){}

	boost::shared_ptr<DeepImage> DeepImage::Ptr() {
					return boost::shared_ptr<DeepImage>(new DeepImage());
				}

	void DeepImage::reset(  int w, int h, int d )
				{
					for (int x = 0; x < mImpl->width; ++x) {
						for (int y = 0; y < mImpl->height; ++y) {
							mImpl->data[mImpl->index(x,y)].clear();
						}
					}
					mImpl->width = w;
					mImpl->height = h;
					mImpl->depth = d;
					mImpl->data.resize(mImpl->width*mImpl->height);
				}

	const DeepPixelBuffer& DeepImage::value( int x, int y) const { return mImpl->data[ mImpl->index(x,y) ]; }

				const DeepPixelBufferVector DeepImage::getDeepPixelBufferVector( int x, int y) { 
					DeepPixelBuffer deeplist = mImpl->data[mImpl->index(x,y)];
					DeepPixelBufferVector pixels(deeplist.begin(), deeplist.end());
					return pixels; 
				}

	DeepPixelBuffer& DeepImage::value(int x, int y ) { return mImpl->data[ mImpl->index(x,y) ]; }
	DeepPixelBuffer& DeepImage::pixel(int x ) { return mImpl->data[x]; }

	const int DeepImage::Width() const { return mImpl->width; }
	const int DeepImage::Height() const { return mImpl->height; }
	const int DeepImage::Depth() const { return mImpl->depth; }



	boost::shared_ptr<Image> DeepImage::flatten() {
	    boost::shared_ptr<Image> image = Image::Ptr();
	    image->reset(mImpl->width, mImpl->height);
	    for (int x = 0; x < mImpl->width; ++x) {
		for (int y = 0; y < mImpl->height; ++y) {
		    Color color(0,0,0,0);
		    DeepPixelBuffer buf = mImpl->data[mImpl->index(x,y)];
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
