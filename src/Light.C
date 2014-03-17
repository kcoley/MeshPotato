#include "MPVolume/Light.h"

namespace MeshPotato {
	namespace MPVolume {
		class FrustumLight::Impl {
			public:
				boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> frustum;
				MeshPotato::MPUtils::Color color;
		};
		boost::shared_ptr<FrustumLight> FrustumLight::Ptr(boost::shared_ptr<FrustumGrid> _frustum, MeshPotato::MPUtils::Color _color) {
			return boost::shared_ptr<FrustumLight>(new FrustumLight(_frustum, _color));
		}
		FrustumLight::FrustumLight(boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> _frustum, MeshPotato::MPUtils::Color _color) :
			mImpl(new FrustumLight::Impl())
		{
			mImpl->frustum = _frustum;
			mImpl->color = _color;
		}
		const FrustumLight::volumeDataType FrustumLight::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
			return exp(-mImpl->frustum->eval(P)) * mImpl->color;
		}
		const FrustumLight::volumeGradType FrustumLight::grad(const MeshPotato::MPUtils::MPVec3 &P) const {
			// TODO: Not implemented yet
		}
	}
}
