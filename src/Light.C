#include "MPVolume/Light.h"
#include "MPVolume/DenseGrid.h"

namespace MeshPotato {
	namespace MPVolume {
		class FrustumLight::Impl {
			public:
				boost::shared_ptr<MeshPotato::MPVolume::FrustumGrid> frustum;
				MeshPotato::MPUtils::Color color;
		};
		VolumeColorPtr FrustumLight::Ptr(boost::shared_ptr<FrustumGrid> _frustum, MeshPotato::MPUtils::Color _color) {
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




		class DenseLight::Impl {
			public:
				boost::shared_ptr<MeshPotato::MPVolume::DenseGrid<float> > grid;
				MeshPotato::MPUtils::Color color;
		};
		VolumeColorPtr DenseLight::Ptr(boost::shared_ptr<DenseGrid<float> > _grid, MeshPotato::MPUtils::Color _color) {
			return boost::shared_ptr<DenseLight>(new DenseLight(_grid, _color));
		}
		DenseLight::DenseLight(boost::shared_ptr<MeshPotato::MPVolume::DenseGrid<float> > _grid, MeshPotato::MPUtils::Color _color) :
			mImpl(new DenseLight::Impl())
		{
			mImpl->grid = _grid;
			mImpl->color = _color;
		}
                void dsm(const VolumeFloatPtr volume) {
                     // Loop through every voxel
                          // compute ray from voxel to light
                          // march along ray to compute DSM value
                }
		const DenseLight::volumeDataType DenseLight::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
			return exp(-mImpl->grid->eval(P)) * mImpl->color;
		}
		const DenseLight::volumeGradType DenseLight::grad(const MeshPotato::MPUtils::MPVec3 &P) const {
			// TODO: Not implemented yet
		}
	}
}
