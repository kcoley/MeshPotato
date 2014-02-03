#ifndef __FRUSTUM_GRID_H__
#define __FRUSTUM_GRID_H__
#include "MPUtils/Vector.h"
#include "MPVolume/VDBVolumeGrid.h"
#include <openvdb/Grid.h>
#include <openvdb/tools/Interpolation.h>
#include <openvdb/tools/RayIntersector.h>
#include "MPUtils/Camera.h"
#include <iostream>
#include "MPUtils/ProgressMeter.h"
namespace MeshPotato {
namespace MPVolume {
class FrustumGrid : public Volume<float> {
public:
	static boost::shared_ptr<FrustumGrid > Ptr(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count=100, openvdb::Coord::ValueType z_count=100) { return boost::shared_ptr<FrustumGrid >(new FrustumGrid(_cam, _bbox));}
	FrustumGrid(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, openvdb::BBoxd _bbox) : cam(_cam), bbox(_bbox), grid(openvdb::FloatGrid::create(0)), frustumTransform(cam->createFrustumTransform(bbox)), interpolator(grid->constTree(), *frustumTransform), accessor(grid->getAccessor()) {
		grid->setTransform(frustumTransform);
	}
	virtual const float eval(const MeshPotato::MPUtils::MPVec3 &P) const {
		return interpolator.wsSample(P);	

	}
	void set(const openvdb::Coord &ijk, float value) {
		// index space coordinate
		accessor.setValue(ijk, value);
	}
	//! World to index space conversion of a point
	const openvdb::Coord worldToIndex(const MeshPotato::MPUtils::MPVec3 &P) {
		return frustumTransform->worldToIndexCellCentered(P);
	
	}
	MeshPotato::MPUtils::MPVec3 indexToWorld(const openvdb::Coord &ijk) {
		return frustumTransform->indexToWorld(ijk);
	}
	MeshPotato::MPUtils::MPVec3 indexToWorld(const MeshPotato::MPUtils::MPVec3 &ijk) {
		return frustumTransform->indexToWorld(ijk);
	}
	const void dsm(const VolumeFloatPtr density_grid, float K) {
		
		openvdb::Coord ijk;
		int dim = 1;
		int &i = ijk[0], &j = ijk[1], &k = ijk[2];
		int xSize = bbox.max().x() - bbox.min().x();
		int ySize = bbox.max().y() - bbox.min().y();
		int zSize = bbox.max().z() - bbox.min().z();
			MeshPotato::MPUtils::ProgressMeter meter(bbox.max().y(), "dsm");
			for (j = bbox.min().y(); j < bbox.max().y(); ++j) {
					for (i = bbox.min().x(); i < bbox.max().x(); ++i) {
						double x = (double)i/(xSize - 1);
						double y = (double)j/(ySize - 1);
						MPVec3 X = cam->eye();
						MPVec3 d = cam->view(x,y);

					
						double deltaS = ((cam->farPlane() - cam->nearPlane())/(cam->view().dot(d)))/(zSize - 1);
						double value = 0;
						X += cam->nearPlane()/((cam->view().dot(d)))*d;

				for (k = bbox.min().z(); k < bbox.max().z(); ++k) {
					X += d * deltaS;

					MPVec3 tempX = frustumTransform->indexToWorld(ijk);
					 double val = density_grid->eval(tempX);

					if (val > 0) {
						value += val * deltaS*K;
						accessor.setValue(ijk, value);
				}

				}

			}
				meter.update();
		}

	}
		const void dsm(openvdb::FloatGrid::Ptr density_grid, float K) {
		// Set the intersector to be the vdb density grid (assuming fog volume)
		openvdb::tools::VolumeRayIntersector<openvdb::FloatGrid> intersector(*density_grid);
		// set the interpolator to the vdb density grid (assuming fog volume)
		openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator(density_grid->constTree(), density_grid->transform());
		openvdb::Coord ijk;
		double t0,t1;
		int dim = 1;
		int &i = ijk[0], &j = ijk[1], &k = ijk[2];
		int xSize = bbox.max().x() - bbox.min().x();
		int ySize = bbox.max().y() - bbox.min().y();
		int zSize = bbox.max().z() - bbox.min().z();
			MeshPotato::MPUtils::ProgressMeter meter(bbox.max().y(), "dsm");
			for (j = bbox.min().y(); j < bbox.max().y(); ++j) {
					for (i = bbox.min().x(); i < bbox.max().x(); ++i) {
						double x = (double)i/(xSize - 1);
						double y = (double)j/(ySize - 1);
						MPVec3 X = cam->eye();
						MPVec3 d = cam->view(x,y);

					
						double deltaS = ((cam->farPlane() - cam->nearPlane())/(cam->view().dot(d)))/(zSize - 1);
						double value = 0;
						X += cam->nearPlane()/((cam->view().dot(d)))*d;
						double time = 0;	
						MeshPotato::MPUtils::MPRay ray(X,d);
						// Use the volume ray intersector to determine the index coordinates
						if (intersector.setWorldRay(ray)) {
							// If we hit the density...
							while (int n = intersector.march(t0, t1)) {
								time = t0;
								// Make sure t1 is not further than the far plane of the frustum
							//	if (nindex.z() < cam->farPlane()) {									
								//	t1 = min(t1, cam->farPlane());
									if (n == 2) { // unknown values
										while (time < t1) {
										//	MPVec3 tempX = frustumTransform->indexToWorld(ijk);
											double val = interpolator.wsSample(ray(time));
											if (val > 0) {
												value += val * deltaS*K;
											}
												accessor.setValue(worldToIndex(ray(time)), value);
											//}
											time += deltaS;
										}
									}
									else if (n == 1) { // constant value
					 					double val = interpolator.wsSample(ray(time));
					 					if (val < 0) val = 0;
										while (time < t1) {
											value += val * deltaS*K;
					 						accessor.setValue(worldToIndex(ray(time)), value);
											time += deltaS;
										}

									}
						}
					}
					
			}
			meter.update();
		}
		}

	virtual const MeshPotato::MPUtils::MPVec3 grad(const MeshPotato::MPUtils::MPVec3 &P) const {}
private:
	boost::shared_ptr<MeshPotato::MPUtils::Camera> cam;
openvdb::BBoxd bbox;
openvdb::FloatGrid::Ptr grid;
openvdb::math::Transform::Ptr frustumTransform;	
openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator;
openvdb::FloatGrid::Accessor accessor;
};


class FrustumGrid2 : public Volume<float> { 
public:
	static boost::shared_ptr<FrustumGrid2 > Ptr(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count=100, openvdb::Coord::ValueType z_count=100) { return boost::shared_ptr<FrustumGrid2 >(new FrustumGrid2(_cam, _bbox, x_count, z_count));}
FrustumGrid2(boost::shared_ptr<MeshPotato::MPUtils::Camera> _cam, openvdb::BBoxd _bbox, openvdb::Coord::ValueType x_count=100, openvdb::Coord::ValueType z_count=100) :
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
			z_count)
	), 
	frustum_map_ptr(frustum_map.copy()),
	frustumTransform(frustum_map_ptr), 
	interpolator(grid->constTree(), frustumTransform), 
	accessor(grid->getAccessor()),
	fastSampler(accessor, frustumTransform)
	 {
	//	openvdb::math::NonlinearFrustumMap(camera.eye(), camera.view(), camera.up(), camera.aspectRatio(), camera.nearPlane(), camera.farPlane(), x_count,z_count)
	    grid->setTransform(frustumTransform.copy());

	}     
	const float eval(const MeshPotato::MPUtils::MPVec3 &P) const {
		return  fastSampler.wsSample(P);     
	}     
	const void set(const openvdb::Coord &ijk, float value) {         
		accessor.setValue(ijk, value);
	}     
	MeshPotato::MPUtils::MPVec3 indexToWorld(const openvdb::Coord &ijk) {
		openvdb::Vec3d vec(1,2,3);
		return frustumTransform.indexToWorld(ijk);     
	} 
	virtual const MeshPotato::MPUtils::MPVec3 grad(const MeshPotato::MPUtils::MPVec3 &P) const {}
	openvdb::CoordBBox getBBox() { return grid->evalActiveVoxelBoundingBox();}

	const void stamp(const VolumeFloatPtr density_grid) {
		std::cout << "stamp function" << std::endl;
		std::cout << frustumTransform << std::endl;
			 // Get a voxel accessor.
	// Compute the signed distance from the surface of the sphere of each
	// voxel within the bounding box and insert the value into the grid
	// if it is smaller in magnitude than the background value.
		openvdb::Coord ijk;
		int dim = 1;
		int &i = ijk[0], &j = ijk[1], &k = ijk[2];
		



		for (i = bbox.min().x(); i < bbox.max().x(); ++i) {
			for (j = bbox.min().y(); j < bbox.max().y(); ++j) {
				for (k = bbox.min().z(); k < bbox.max().z(); ++k) {
				// The distance from the sphere surface in voxels
					const float val = density_grid->eval(grid->indexToWorld(ijk));
					// Convert the floating-point distance to the grid's value type.
					// Only insert distances that are smaller in magnitude than
					// the background value.
					if (val <= 0) {continue;
						
					}
					

					// Set the distance for voxel (i,j,k).
					accessor.setValue(ijk, val);

				}
			}
		}

	}

		const void dsm(const VolumeFloatPtr density_grid, double K) {
		int test = 0;
		int count = 0;
		openvdb::Coord ijk;
		int dim = 1;
		int &i = ijk[0], &j = ijk[1], &k = ijk[2];
		int xSize = bbox.max().x() - bbox.min().x();
		int ySize = bbox.max().y() - bbox.min().y();
		int zSize = bbox.max().z() - bbox.min().z();
			MeshPotato::MPUtils::ProgressMeter meter(bbox.max().y(), "dsm");
			for (j = bbox.min().y(); j < bbox.max().y(); ++j) {
					for (i = bbox.min().x(); i < bbox.max().x(); ++i) {
						double x = (double)i/(xSize - 1);
						double y = (double)j/(ySize - 1);
						MPVec3 X = camera->eye();
						MPVec3 d = camera->view(x,y);
						test = 0;
					
						double deltaS = ((camera->farPlane() - camera->nearPlane())/(camera->view().dot(d)))/(zSize - 1);
						double value = 0;
						X += camera->nearPlane()/((camera->view().dot(d)))*d;

				for (k = bbox.min().z(); k < bbox.max().z(); ++k) {
					X += d * deltaS;
					MPVec3 tempX = indexToWorld(ijk);
					 double val = density_grid->eval(tempX);

					if (val > 0) {
						value += val * deltaS * K;
						test = 1;
						count++;
					}	
					if(test == 0 )
						accessor.setValue(ijk, 0.1);
					else
						accessor.setValue(ijk, value);


				}
			}
			meter.update();
		}
		std::cout<<"count: "<<count<<std::endl;
	}
private:
openvdb::FloatGrid::Ptr grid;     
boost::shared_ptr<MeshPotato::MPUtils::Camera> camera;
openvdb::BBoxd bbox;
openvdb::math::NonlinearFrustumMap frustum_map;
openvdb::math::MapBase::Ptr frustum_map_ptr;
openvdb::math::Transform frustumTransform;
openvdb::tools::GridSampler<openvdb::FloatTree, openvdb::tools::BoxSampler> interpolator;     
openvdb::FloatGrid::Accessor accessor;
openvdb::tools::GridSampler<openvdb::FloatGrid::Accessor, openvdb::tools::BoxSampler> fastSampler;
};

}
}
#endif // __FRUSTUM_GRID_H__
