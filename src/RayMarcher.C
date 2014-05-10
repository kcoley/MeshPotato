#include "MPVolume/RayMarcher.h"
#include <MPVolume/FrustumGrid.h>
#include "MPUtils/DeepImage.h"
#include "MPUtils/Image.h"
#include "MPUtils/OIIOFiles.h"
#include "MPUtils/AttributeTable.h"


using namespace MeshPotato::MPUtils;
namespace MeshPotato {
namespace MPVolume {
class VDBRayMarcher::Impl{
public:
	openvdb::FloatGrid::Ptr grid;
	VolumeColorPtr dsm;
	boost::shared_ptr<MeshPotato::MPUtils::Image> image;
	boost::shared_ptr<MeshPotato::MPUtils::DeepImage> deepimage;
	boost::shared_ptr<MeshPotato::MPUtils::Camera> camera;
	AttributeTable table;
};
VDBRayMarcher::VDBRayMarcher(openvdb::FloatGrid::Ptr _grid, 
	     VolumeColorPtr _dsm,
	     boost::shared_ptr<MeshPotato::MPUtils::Camera> _camera, 
	     const AttributeTable &_table 
	     ) : 
	     mImpl(new VDBRayMarcher::Impl)
{
	mImpl->grid = _grid;
	mImpl->dsm = _dsm;
	mImpl->image = MeshPotato::MPUtils::Image::Ptr();
	mImpl->deepimage = MeshPotato::MPUtils::DeepImage::Ptr();
	mImpl->camera = _camera;
	mImpl->table = _table;
	mImpl->table.addDoubleAttr("step", 1);
	mImpl->table.addDoubleAttr("K", 1);
	mImpl->table.addIntAttr("NX", 960);
	mImpl->table.addIntAttr("NY", 540);
	mImpl->table.addIntAttr("deep", 0);
	mImpl->table.mergeTable(_table);
	mImpl->image->reset(mImpl->table.findIntAttr("NX"), mImpl->table.findIntAttr("NY"));
	mImpl->deepimage->reset(mImpl->table.findIntAttr("NX"), mImpl->table.findIntAttr("NY"));
}

const MeshPotato::MPUtils::Color VDBRayMarcher::L(MPRay &ray, IntersectorT &intersector2, SamplerType &interpolator2) const {
	Color _L = Color(0,0,0,0);
	float deltaT;
	float _T = 1.0f;
	float time = 0.0f;
        double step = mImpl->table.findDoubleAttr("step");
        double K = mImpl->table.findDoubleAttr("K");	
	if (!intersector2.setWorldRay(ray)) return _L;
	double t0 = 0, t1 = 0;
	while (int n = intersector2.march(t0, t1)) {
		for (double time = step*ceil(t0/step); time <= t1; time += step) {
				MPVec3 P = intersector2.getWorldPos(time);
				double density = interpolator2.wsSample(P);

				if (density > 0) {
					deltaT = exp(-K*density*step);
					Color CM(1,1,1,1);
			                Color CI = mImpl->dsm->eval(P);
			                Color CS = CI*CM;
	            
					_L += ((CS)*_T*(1.0 - deltaT));
					_T *=deltaT;
					if (_T < 0.0000001) return Color(_L[0],_L[1],_L[2],1.0 - _T);
				}
		}

	}
	return Color(_L[0],_L[1],_L[2],1.0 - _T);
}
const MeshPotato::MPUtils::DeepPixelBuffer VDBRayMarcher::deepL(MPRay &ray, IntersectorT &intersector2, SamplerType &interpolator2) const {
	float deltaT;
	MeshPotato::MPUtils::DeepPixelBuffer deepPixelBuf;

	if (!intersector2.setWorldRay(ray)) return deepPixelBuf;
        double step = mImpl->table.findDoubleAttr("step");	
	double K = mImpl->table.findDoubleAttr("K");
	double t0 = 0, t1 = 0;
	while (int n = intersector2.march(t0, t1)) {
		for (float time = step*ceil(t0/step); time <= t1; time += step) {
				MPVec3 P = intersector2.getWorldPos(time);
				double density = interpolator2.wsSample(P);

				if (density > 0) {
					deltaT = exp(-K*density*step);
					Color CM(1,1,1,1);
	                		Color CI = mImpl->dsm->eval(P);
					Color CS = CI*CM;

					MeshPotato::MPUtils::DeepPixel deepPixel;

	            
					Color _L = ((CS)*(1 - deltaT));
					deepPixel.color = _L;
					deepPixel.color[3] = 1.0 - deltaT;
					deepPixel.depth_front = (P - mImpl->camera->eye()).length();//(ray(time)).dot(cam.view());
					deepPixelBuf.push_back(deepPixel);
				}
		}

	}
	return deepPixelBuf;
}
void VDBRayMarcher::operator() (const tbb::blocked_range<size_t>& r) const {
	for (size_t j = r.begin(), je = r.end(); j < je; ++j) {
	AccessorType accessor2(mImpl->grid->getConstAccessor());
	SamplerType interpolator2(accessor2, mImpl->grid->transform());
		// Create an intersector for each thread
		IntersectorT intersector2(*(mImpl->grid));
        int NX = mImpl->table.findIntAttr("NX");
        int NY = mImpl->table.findIntAttr("NY");
		for (size_t i = 0, ie = NX; i < ie; ++i) {
			MeshPotato::MPUtils::MPRay ray;
			double x = (double)i/(NX - 1.0);
               		double y = (double)j/(NY - 1.0);
			ray = mImpl->camera->getRay(x,y);

			const DeepPixelBuffer deepColor = deepL(ray, intersector2, interpolator2);
//			std::vector<float> &pixel = image->pixel(i,j);
//			pixel[0] = c[0];
//			pixel[1] = c[1];
//			pixel[2] = c[2];
//			pixel[3] = c[3];
			mImpl->deepimage->value(i,j) = deepColor;
		}
	}
}
void VDBRayMarcher::render(bool threaded) const {
	tbb::blocked_range<size_t> range(0, mImpl->table.findIntAttr("NY"));
	threaded ? tbb::parallel_for(range, *this) : (*this)(range);
}
void VDBRayMarcher::writeImage(const std::string outputImage) {
	boost::shared_ptr<Image> tempimage = mImpl->deepimage->flatten();
	MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), *tempimage);
//	MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), *image);
	 MeshPotato::MPUtils::writeOIIOImage(("deep"+outputImage).c_str(), *(mImpl->deepimage));
}

class RayMarcher::Impl {
public:
MeshPotato::MPVolume::VolumeFloatPtr grid;
VolumeColorPtr dsm;
float K, step;
boost::shared_ptr<Camera> camera;
MPUtils::BBox bbox;
boost::shared_ptr<MPUtils::Image> image;
boost::shared_ptr<MPUtils::DeepImage> deepimage;
MeshPotato::MPUtils::AttributeTable table;
};

RayMarcher::RayMarcher(VolumeFloatPtr _grid, VolumeColorPtr _dsm, boost::shared_ptr<Camera> _camera, const AttributeTable &_table) : mImpl(new RayMarcher::Impl) {
mImpl->grid = _grid;
mImpl->dsm = _dsm;
mImpl->camera = _camera;
mImpl->table.addDoubleAttr("step", 1);
mImpl->table.addDoubleAttr("K", 1);
mImpl->table.addIntAttr("NX", 960);
mImpl->table.addIntAttr("NY", 540);
mImpl->table.addIntAttr("deep", 0);
mImpl->table.addVectorAttr("minBB", MeshPotato::MPUtils::MPVec3(-1,-1,-1));
mImpl->table.addVectorAttr("maxBB", MeshPotato::MPUtils::MPVec3(1,1,1));
mImpl->table.mergeTable(_table);
mImpl->image = MeshPotato::MPUtils::Image::Ptr();
mImpl->deepimage = MeshPotato::MPUtils::DeepImage::Ptr();
mImpl->image->reset(mImpl->table.findIntAttr("NX"), mImpl->table.findIntAttr("NY"));
mImpl->deepimage->reset(mImpl->table.findIntAttr("NX"), mImpl->table.findIntAttr("NY"));
mImpl->bbox = MeshPotato::MPUtils::BBox(
	mImpl->table.findVectorAttr("minBB"),
	mImpl->table.findVectorAttr("maxBB")
	);

}
MeshPotato::MPUtils::Color RayMarcher::L(MPRay &ray) const {
        Color _L = Color(0,0,0,0);
        float deltaT;
        float _T = 1.0f;
        float time = 0.0f;
        double t0 = ray.t0(), t1 = ray.t1();
	time = t0;
		double step = mImpl->table.findDoubleAttr("step");
		double K = mImpl->table.findDoubleAttr("K");
        while (time < t1) {
                        time += step;
                        while (time < t1) {
                        MeshPotato::MPUtils::MPVec3 P = ray(time);
                        float density = mImpl->grid->eval(P);
                        if (density > 0) {
                                deltaT = exp(-K * density * step);
				Color CI = mImpl->dsm->eval(P); 
				Color CM(1,1,1,1);
				Color CS = CI*CM;
//                              Color CS = density * (Color(1.0, 1.0, 1.0, 1.0) * exp(-dsm->eval(P) * K));
                                _L += ((CS) * _T * (1.0 - deltaT));
                                _T *=deltaT;
                                if (_T < 0.0000001) break;
                        }
                        time += step;
                	}
        }
        return Color(_L[0],_L[1],_L[2],1.0 - _T);
}
void RayMarcher::operator() (const tbb::blocked_range<size_t>& r) const {
	for (size_t j = r.begin(), je = r.end(); j < je; ++j) {
		for (size_t i = 0, ie = mImpl->image->Width(); i < ie; ++i) {
			MeshPotato::MPUtils::MPRay ray;
			double x = (double)i/(mImpl->image->Width() - 1.0);
               		double y = (double)j/(mImpl->image->Height() - 1.0);
			ray = mImpl->camera->getRay(x,y);

//			const DeepPixelBuffer deepColor = deepL(ray, intersector2, interpolator2);
			// Bounding Box Check)
			double t0, t1;
			if (ray.intersects(mImpl->bbox,t0, t1)) {
				ray.setMinTime(t0);
				ray.setMaxTime(t1);
			const Color c = L(ray);
			std::vector<float> &pixel = mImpl->image->pixel(i,j);
			pixel[0] = c[0];
			pixel[1] = c[1];
			pixel[2] = c[2];
			pixel[3] = c[3];
		}
//			deepimage->value(i,j) = deepColor;
		}
	}
}
void RayMarcher::render(bool threaded) const {
        tbb::blocked_range<size_t> range(0, mImpl->image->Height());
        threaded ? tbb::parallel_for(range, *this) : (*this)(range);
}
void RayMarcher::writeImage(const std::string outputImage) {
//        Image tempimage = deepimage->flatten();
  //      MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), tempimage);
      MeshPotato::MPUtils::writeOIIOImage(outputImage.c_str(), *(mImpl->image));
  //       MeshPotato::MPUtils::writeOIIOImage(("deep"+outputImage).c_str(), *deepimage);
}


}
}
