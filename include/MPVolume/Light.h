#include "FrustumGrid.h"
#include <boost/shared_ptr.hpp>
namespace MeshPotato {
	namespace MPVolume {
	class Light : public Volume<MPUtils::Color> {
	};
	class FrustumLight : public Light {
	public:
	static VolumeColorPtr Ptr(
		boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> _frustum, 
		MeshPotato::MPUtils::Color _color);

		FrustumLight(boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> _frustum, 
		MeshPotato::MPUtils::Color _color);

		virtual const FrustumLight::volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
		virtual const FrustumLight::volumeGradType grad (const MeshPotato::MPUtils::MPVec3 &P) const;
		private:
		class Impl;
		boost::shared_ptr<Impl> mImpl;
	};
	}
}
