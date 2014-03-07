#include "MPVolume/ImplicitShape.h" 
namespace MeshPotato {
namespace MPVolume {
//TODO: Implement Copy Constructor and Assignment Operator
class ImplicitSphere::Impl {
public:
	float R;
	MeshPotato::MPUtils::MPVec3 C;
};

boost::shared_ptr<ImplicitSphere> ImplicitSphere::Ptr(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C) {
	return boost::shared_ptr<ImplicitSphere>(new ImplicitSphere(_R, _C));	
}
ImplicitSphere::ImplicitSphere(const float &_R,const MeshPotato::MPUtils::MPVec3 &_C) : mImpl(new ImplicitSphere::Impl()) {
mImpl->R = _R; 
mImpl->C = _C;
}

ImplicitSphere::~ImplicitSphere() {}
const ImplicitSphere::volumeDataType ImplicitSphere::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
	return mImpl->R - (P - mImpl->C).length();
}
const ImplicitSphere::volumeGradType ImplicitSphere::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}

boost::shared_ptr<PyroclasticSphere> PyroclasticSphere::Ptr(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms) {
 return boost::shared_ptr<PyroclasticSphere>(new PyroclasticSphere(_R, _C, _noiseparms));
}

class PyroclasticSphere::Impl {
public:
	float R;
	MeshPotato::MPUtils::MPVec3 C;
	MeshPotato::MPNoise::Noise_t noiseparms;
        MeshPotato::MPNoise::FractalSum<MeshPotato::MPNoise::PerlinNoiseGustavson> perlin;
        MeshPotato::MPNoise::Noise* noise;
};
PyroclasticSphere::PyroclasticSphere(const float &_R, const MeshPotato::MPUtils::MPVec3 &_C, const MeshPotato::MPNoise::Noise_t &_noiseparms) : mImpl(new PyroclasticSphere::Impl()) {
	mImpl->R = _R;
	mImpl->C = _C;
	mImpl->noiseparms = _noiseparms;
        mImpl->perlin.setParameters(mImpl->noiseparms);
        mImpl->noise = &mImpl->perlin;
 }
PyroclasticSphere::~PyroclasticSphere() {
}
const PyroclasticSphere::volumeDataType PyroclasticSphere::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
                                        return mImpl->R - (P - mImpl->C).length() + pow(fabs(mImpl->noise->eval((P - mImpl->C)/(P - mImpl->C).length())), mImpl->noiseparms.gamma)*1.0;
}

const PyroclasticSphere::volumeGradType PyroclasticSphere::grad(const MeshPotato::MPUtils::MPVec3& P) const {
                                        return (mImpl->C-P).unit();
}


}
}
