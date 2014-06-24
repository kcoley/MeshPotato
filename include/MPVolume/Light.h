#include "FrustumGrid.h"
#include <boost/shared_ptr.hpp>
#include "DenseGrid.h"
namespace MeshPotato {
	namespace MPVolume {
		/// Base class for lights

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

        class DenseLight : public Light {
        public:
        static VolumeColorPtr Ptr(
               boost::shared_ptr<MeshPotato::MPVolume::DenseGrid<float> > _grid,
               MeshPotato::MPUtils::Color _color);

               DenseLight(boost::shared_ptr<MeshPotato::MPVolume::DenseGrid<float> > _grid,
               MeshPotato::MPUtils::Color _color);

               virtual const DenseLight::volumeDataType eval(const MeshPotato::MPUtils::MPVec3 &P) const;
               virtual const DenseLight::volumeGradType grad(const MeshPotato::MPUtils::MPVec3 &P) const;
        private:
        class Impl;
        boost::shared_ptr<Impl> mImpl;
        };
	}
}
