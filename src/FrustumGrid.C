#include "MeshPotato/MPVolume/FrustumGrid.h"
#include "MeshPotato/MPUtils/Vector.h"
#include "MeshPotato/MPVolume/VDBVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>
#include "MeshPotato/MPUtils/Camera.h"
#include <iostream>
#include "MeshPotato/MPUtils/ProgressMeter.h"
namespace MeshPotato {
namespace MPVolume {
	class FrustumGrid::Impl {
	public:
		Impl(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, 
			openvdb::BBoxd _bbox, 
			openvdb::Coord::ValueType x_count, 
			openvdb::Coord::ValueType z_count) : 
				grid(openvdb::FloatGrid::create(0)),
				camera(_cam),
				bbox(_bbox),
				frustum_map(openvdb::math::NonlinearFrustumMap(
					camera->eye(),
					camera->view(),
					camera->up()*tan(camera->fov()*0.5*M_PI/180.0)*camera->nearPlane(),
					camera->aspectRatio(),
					camera->nearPlane(),
					camera->farPlane(),
				//	abs(camera->farPlane() - camera->nearPlane()),
					x_count,
					z_count)),
					frustum_map_ptr(frustum_map.copy()),
					frustumTransform(frustum_map_ptr),
					interpolator(grid->constTree(), frustumTransform),
					accessor(grid->getAccessor()),
					accessor2(grid->getConstAccessor()),
					fastSampler(accessor2, frustumTransform) {}


		openvdb::FloatGrid::Ptr grid;
		boost::shared_ptr<MeshPotato::MPUtils::Camera> camera;
		openvdb::BBoxd bbox;
		openvdb::math::NonlinearFrustumMap frustum_map;
		openvdb::math::MapBase::Ptr frustum_map_ptr;
		openvdb::math::Transform frustumTransform;
		openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator;
		openvdb::FloatGrid::Accessor accessor;
		openvdb::FloatGrid::ConstAccessor accessor2;
		openvdb::tools::GridSampler<openvdb::FloatGrid::ConstAccessor, openvdb::tools::BoxSampler> fastSampler;
	};

	/// Creates a frustum-shaped grid
boost::shared_ptr<FrustumGrid > FrustumGrid::Ptr(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, 
												openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count, 
												openvdb::Coord::ValueType z_count) { 
	return boost::shared_ptr<FrustumGrid >(new FrustumGrid(_cam, _bbox, x_count, z_count));
}

FrustumGrid::FrustumGrid(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count, openvdb::Coord::ValueType z_count) : mImpl(new Impl(_cam, _bbox, x_count, z_count))
{
	//	openvdb::math::NonlinearFrustumMap(camera.eye(), camera.view(), camera.up(), camera.aspectRatio(), camera.nearPlane(), camera.farPlane(), x_count,z_count)
	    mImpl->grid->setTransform(mImpl->frustumTransform.copy());

}

	const float FrustumGrid::eval(const MeshPotato::MPUtils::MPVec3 &P) const {
//		return  fastSampler.wsSample(P);
		return  mImpl->interpolator.wsSample(P);
	}
	const void FrustumGrid::set(const openvdb::Coord &ijk, float value) {
		mImpl->accessor.setValue(ijk, value);
	}
	MeshPotato::MPUtils::MPVec3 FrustumGrid::indexToWorld(const openvdb::Coord &ijk) {
		return mImpl->frustumTransform.indexToWorld(ijk);
	}
	const MeshPotato::MPUtils::MPVec3 FrustumGrid::grad(const MeshPotato::MPUtils::MPVec3 &P) const {}
	
	//openvdb::CoordBBox getBBox() { return grid->evalActiveVoxelBoundingBox();}

	const void FrustumGrid::stamp(const VolumeFloatPtr density_grid) {
		std::cout << "stamp function" << std::endl;
		std::cout << mImpl->frustumTransform << std::endl;
			 // Get a voxel accessor.
	// Compute the signed distance from the surface of the sphere of each
	// voxel within the bounding box and insert the value into the grid
	// if it is smaller in magnitude than the background value.
		openvdb::Coord ijk;
		int dim = 1;
		int &i = ijk[0], &j = ijk[1], &k = ijk[2];




		for (i = mImpl->bbox.min().x(); i < mImpl->bbox.max().x(); ++i) {
			for (j = mImpl->bbox.min().y(); j < mImpl->bbox.max().y(); ++j) {
				for (k = mImpl->bbox.min().z(); k < mImpl->bbox.max().z(); ++k) {
				// The distance from the sphere surface in voxels
					const float val = density_grid->eval(mImpl->grid->indexToWorld(ijk));
					// Convert the floating-point distance to the grid's value type.
					// Only insert distances that are smaller in magnitude than
					// the background value.
					if (val <= 0) {continue;

					}


					// Set the distance for voxel (i,j,k).
					mImpl->accessor.setValue(ijk, val);

				}
			}
		}

	}

		const void FrustumGrid::dsm(const VolumeFloatPtr density_grid, double K) {
		int count = 0;
		openvdb::Coord ijk;
		int dim = 1;
		int &i = ijk[0], &j = ijk[1], &k = ijk[2];
		int xSize = mImpl->bbox.max().x() - mImpl->bbox.min().x();
		int ySize = mImpl->bbox.max().y() - mImpl->bbox.min().y();
		int zSize = mImpl->bbox.max().z() - mImpl->bbox.min().z();
			MeshPotato::MPUtils::ProgressMeter meter(mImpl->bbox.max().y(), "dsm");
			for (j = mImpl->bbox.min().y(); j < mImpl->bbox.max().y(); ++j) {
					for (i = mImpl->bbox.min().x(); i < mImpl->bbox.max().x(); ++i) {
						double x = (double)i/(xSize - 1.0);
						double y = (double)j/(ySize - 1.0);
						MPVec3 X = mImpl->camera->eye();
						MPVec3 d = mImpl->camera->view(x,y);

						double deltaS = ((mImpl->camera->farPlane() - mImpl->camera->nearPlane())/(mImpl->camera->view().dot(d)))/(zSize - 1.0);
						double value = 0;
						X += mImpl->camera->nearPlane()/((mImpl->camera->view().dot(d)))*d;

				for (k = mImpl->bbox.min().z(); k < mImpl->bbox.max().z(); ++k) {
					X += d * deltaS;
					MPVec3 tempX = indexToWorld(ijk);
					 double val = density_grid->eval(tempX);

					if (val > 0) {
						value += val * deltaS * K;
						count++;
					}
					mImpl->accessor.setValue(ijk, value);


				}
			}
			meter.update();
		}
		std::cout<<"count: "<<count<<std::endl;
	}


}
}

