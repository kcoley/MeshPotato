#include "FrustumGrid.h"
namespace MeshPotato {
	namespace MPVolume {
		class Light : public Volume<Color> {
		};
		class FrustumLight : public Light {
			public:
				static boost::shared_ptr<FrustumLight> Ptr(boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> &_frustum, MeshPotato::MPUtils::Color _color) { return boost::shared_ptr<FrustumLight>(new FrustumLight(_frustum, _color));}

				FrustumLight(boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> &_frustum, MeshPotato::MPUtils::Color _color) : frustum(_frustum), color(_color) {}

				virtual const volumeDataType  eval(const MeshPotato::MPUtils::MPVec3 &P) const {
					return exp(-frustum->eval(P)) * color;
				}
				virtual const volumeGradType grad (const MeshPotato::MPUtils::MPVec3 &P) const {}
			private:
				boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> frustum;
				MeshPotato::MPUtils::Color color;
		};
	}
}
